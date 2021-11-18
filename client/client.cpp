#include "client.h"

Client::Client(QWidget* parent)
    : QStackedWidget(parent)
{
    login = new Login;
    addWidget(login);
    talkie = new Talkie;
    addWidget(talkie);

    connect(&socket, &QTcpSocket::readyRead, this, &Client::onRead);
    connect(&socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);

    connect(login, &Login::connectToHost, this, &Client::connectToHost);

    connect(talkie, &Talkie::quit, &socket, &QTcpSocket::close);
}

Client::~Client()
{
    socket.close();
}

void Client::onRead()
{
    // TODO play
}

void Client::onConnected()
{
    login->setConnectBtnEnabled(false);
    setCurrentWidget(talkie);
}

void Client::onDisconnected()
{
    login->setConnectBtnEnabled(true);
    setCurrentWidget(login);
}

void Client::connectToHost(QString const& url, int port, QString const& username, QString const& password, QString const& frequency)
{
    if (socket.state() != QAbstractSocket::ConnectedState)
    {
        login->setConnectBtnEnabled(false);
        socket.connectToHost(url, port);
        socket.write(QString("%1,%2,%3").arg(username, password, frequency).toUtf8());
    }
    else
    {
        socket.disconnectFromHost();
    }
}
