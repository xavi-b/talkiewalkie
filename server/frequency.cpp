#include "frequency.h"

Frequency::Frequency(QObject* parent)
    : QObject(parent)
{
}

Frequency::~Frequency()
{
    for (auto& c : clients)
    {
        if (c)
        {
            c->close();
            c->deleteLater();
        }
    }
}

void Frequency::setFrequency(const QString& frequency)
{
    this->frequency = frequency;
}

void Frequency::addClient(QTcpSocket* client)
{
    connect(client, &QTcpSocket::disconnected, this, &Frequency::onDisconnected);
    connect(client, &QTcpSocket::readyRead, this, &Frequency::onRead);
    clients.append(client);
}

void Frequency::onDisconnected()
{
    QTcpSocket* closingSocket = (QTcpSocket*)sender();
    if (closingSocket == speaker)
        speaker = nullptr;
    clients.removeAll(closingSocket);

    if (clients.size() == 0)
    {
        emit emptyFrequency(frequency);
    }
}

void Frequency::onRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();

    if (speaker != nullptr && clients.contains(speaker) && socket != speaker)
    {
        qWarning() << QDateTime::currentDateTimeUtc() << "Someone already speaking";
        return;
    }

    QByteArray data = socket->readAll();

    if (data.contains("STOP"))
    {
        qWarning() << "No speaker";
        speaker = nullptr;
    }
    else if (speaker == nullptr)
    {
        qWarning() << "New speaker";
        speaker = socket;
    }

    //qDebug() << QDateTime::currentDateTimeUtc() << "Data on frequency" << frequency << data.size();

    for (auto const& client : clients)
    {
        if (client != socket)
            client->write(data);
    }
}
