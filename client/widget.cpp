#include "widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent)
{
    connect(&socket, &QTcpSocket::readyRead, this, &Widget::onRead);
    connect(&socket, &QTcpSocket::connected, this, &Widget::onConnected);
    connect(&socket, &QTcpSocket::disconnected, this, &Widget::onDisconnected);
}

Widget::~Widget()
{
    socket.close();
}

void Widget::onRead()
{
    // TODO play
}

void Widget::onConnected()
{
    // TODO mark button as Cancel
    // TODO go to talkie
}

void Widget::onDisconnected()
{
    // TODO mark button as Connect
    // TODO go to login
}

void Widget::connectToHost()
{
    if (socket.state() != QAbstractSocket::ConnectedState)
    {
        // TODO socket.connectToHost(ui.m_addr->text(), SERV_PORT);
        // TODO send username + password + frequency
    }
    else
    {
        socket.disconnectFromHost();
    }
}
