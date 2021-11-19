#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QTcpSocket>
#include <QDateTime>

class Frequency : public QObject
{
    Q_OBJECT
public:
    explicit Frequency(QObject* parent = nullptr);
    ~Frequency();

    void setFrequency(QString const& frequency);
    void addClient(QTcpSocket* client);
    void onDisconnected();
    void onRead();

signals:
    void emptyFrequency(QString const&);

private:
    QVector<QTcpSocket*> clients;
    QString              frequency;
};

#endif // FREQUENCY_H
