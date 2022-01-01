#include "server.h"
#include <QCoreApplication>

Server::Server(ushort port, QObject* parent)
    : QObject(parent)
{
    connect(&serv, &QTcpServer::newConnection, this, &Server::onConnexion);

    QFile file(QCoreApplication::applicationDirPath() + "/users.csv");
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << file.errorString();
    }

    while (!file.atEnd())
    {
        qDebug() << "Loaded user";
        QList<QByteArray> line = file.readLine().split(',');
        qDebug() << line;
        if (line.size() >= 2)
            users[line[0]] = line[1].trimmed();
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

    qDebug() << "New connexion";

    connect(tmpSocket, &QTcpSocket::readyRead, this, &Server::onFirstRead);
}

void Server::onFirstRead()
{
    qDebug() << "First read";

    QTcpSocket* tmpSocket = (QTcpSocket*)sender();
    disconnect(tmpSocket, &QTcpSocket::readyRead, this, &Server::onFirstRead);

    QStringList data = QString(tmpSocket->readAll()).split(',');
    qDebug() << data;

    if (data.size() == 3)
    {
        QString const& username  = data[0];
        QString const& password  = data[1];
        QString const& frequency = data[2];

        if (checkUser(username, password))
        {
            qDebug() << "User checks";

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
        else
            qDebug() << "User does not check";
    }

    tmpSocket->close();
    tmpSocket->deleteLater();
}

bool Server::checkUser(QString const& username, QString const& password) const
{
    return users.contains(username) && users[username] == password;
}
