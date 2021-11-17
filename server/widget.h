#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QMap>
#include <QString>

#include "frequency.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

    void onConnexion();

private:
    QTcpServer                serv;
    QMap<QString, Frequency*> frequencies;
};
#endif // WIDGET_H
