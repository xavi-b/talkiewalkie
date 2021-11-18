#ifndef WIDGET_H
#define WIDGET_H

#include <QStackedWidget>
#include <QTcpSocket>

#include "login.h"
#include "talkie.h"

class Client : public QStackedWidget
{
    Q_OBJECT

public:
    Client(QWidget* parent = nullptr);
    ~Client();

public slots:
    void onRead();
    void onConnected();
    void onDisconnected();
    void connectToHost(QString const& url, int port, QString const& username, QString const& password, QString const& frequency);

private:
    QTcpSocket socket;
    Login*     login;
    Talkie*    talkie;
};
#endif // WIDGET_H
