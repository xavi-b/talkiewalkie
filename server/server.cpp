#include "server.h"

Server::Server(ushort port, QWidget* parent)
    : QWidget(parent)
{
    connect(&serv, &QTcpServer::newConnection, this, &Server::onConnexion);

    QFile file(":/users.csv");
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << file.errorString();
    }

    while (!file.atEnd())
    {
        QList<QByteArray> line = file.readLine().split(',');
        if (line.size() >= 2)
            users[line[0]] = line[1];
    }

    serv.listen(QHostAddress::Any, port);
}

Server::~Server()
{
    serv.close();
    for (auto& e : frequencies)
        e->deleteLater();
}

void Server::onConnexion()
{
    QTcpSocket* tmpSocket = serv.nextPendingConnection();

    connect(tmpSocket, &QTcpSocket::readyRead, this, &Server::onFirstRead);
}

void Server::onFirstRead()
{
    QTcpSocket* tmpSocket = (QTcpSocket*)sender();
    disconnect(tmpSocket, &QTcpSocket::readyRead, this, &Server::onFirstRead);

    QStringList data = QString(tmpSocket->readAll()).split(',');

    if (data.size() == 3)
    {
        QString const& username  = data[0];
        QString const& password  = data[1];
        QString const& frequency = data[2];

        if (checkUser(username, password))
        {
            if (!frequency.isEmpty())
            {
                if (!frequencies.contains(frequency))
                {
                    frequencies[frequency] = new Frequency(this);
                    frequencies[frequency]->setFrequency(frequency);
                    connect(frequencies[frequency], &Frequency::emptyFrequency, this, [=](QString const& frequency) {
                        frequencies[frequency]->deleteLater();
                        frequencies.remove(frequency);
                    });
                }
                frequencies[frequency]->addClient(tmpSocket);
                return;
            }
        }
    }

    tmpSocket->close();
    tmpSocket->deleteLater();
}

bool Server::checkUser(QString const& username, QString const& password) const
{
    return users.contains(username) && users[username] == password;
}
