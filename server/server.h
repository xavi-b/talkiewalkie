#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <QString>
#include <QFile>

#include "frequency.h"

class Server : public QObject
{
    Q_OBJECT

public:
    Server(ushort port, QObject* parent = nullptr);
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
#endif // SERVER_H
