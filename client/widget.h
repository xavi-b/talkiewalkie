#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

public slots:
    void onRead();
    void onConnected();
    void onDisconnected();
    void connectToHost();

private:
    QTcpSocket socket;
};
#endif // WIDGET_H
