#include "client.h"

Client::Client(QWidget* parent)
    : QStackedWidget(parent)
{
    login = new Login;
    addWidget(login);
    talkie = new Talkie;
    addWidget(talkie);

    effect.setSource(QUrl("qrc:/beep.wav"));
    effect.setVolume(0.5f);

    connect(&socket, &QTcpSocket::readyRead, this, &Client::onRead);
    connect(&socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(&socket, &QTcpSocket::errorOccurred, this, [=]() {
        login->setConnectBtnEnabled(true);
        qCritical() << socket.errorString();
    });

    connect(login, &Login::connectToHost, this, &Client::connectToHost);

    connect(talkie, &Talkie::send, &socket, QOverload<QByteArray const&>::of((&QTcpSocket::write)));
    connect(talkie, &Talkie::quit, &socket, &QTcpSocket::close);
}

Client::~Client()
{
    socket.close();
}

void Client::onRead()
{
    QByteArray data = socket.readAll();
    if (data.contains("BEEP"))
    {
        data.remove(data.indexOf("BEEP"), 5);
        effect.play();
    }
    talkie->play(data);
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
