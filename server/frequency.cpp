#include "frequency.h"

Frequency::Frequency(QObject* parent) : QObject(parent)
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
    clients.append(client);
}

void Frequency::onDisconnected()
{
    QTcpSocket* closingSocket = (QTcpSocket*)sender();
    clients.removeAll(closingSocket);

    if (clients.size() == 0)
    {
        emit emptyFrequency(frequency);
    }
}
