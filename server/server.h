#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QMap>
#include <QString>
#include <QFile>

#include "frequency.h"

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(ushort port, QWidget* parent = nullptr);
    ~Server();

    void onConnexion();
    void onFirstRead();

protected:
    virtual bool checkUser(QString const& username, QString const& password) const;

private:
    QTcpServer                serv;
    QMap<QString, Frequency*> frequencies;
    QMap<QString, QString>    users;
};
#endif // WIDGET_H
