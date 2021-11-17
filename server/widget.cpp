#include "widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent)
{
    connect(&serv, &QTcpServer::newConnection, this, &Widget::onConnexion);

    // TODO serv.listen(QHostAddress::Any, SERV_PORT);
}

Widget::~Widget()
{
    serv.close();
    for (auto& e : frequencies)
        e->deleteLater();
}

void Widget::onConnexion()
{
    QTcpSocket* tmpSocket = serv.nextPendingConnection();

    connect(tmpSocket, &QTcpSocket::readyRead, this, [=]() {
        // TODO check username + password
        // TODO get frequency
        QString frequency;
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
        }
        else
        {
            tmpSocket->close();
            tmpSocket->deleteLater();
        }
    });
}
