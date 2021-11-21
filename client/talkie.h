#ifndef TALKIE_H
#define TALKIE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QKeyEvent>
#include <QPainter>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioSource>
#include <QAudioSink>
#include <QDateTime>
#include <QTcpSocket>

class Talkie : public QWidget
{
    Q_OBJECT
public:
    explicit Talkie(QTcpSocket* socket, QWidget* parent = nullptr);
    void record();
    void stop();
    void play();

signals:
    void send(QByteArray const&);
    void quit();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void initializeInputAudio(const QAudioDevice& deviceInfo);
    void initializeOutputAudio(const QAudioDevice& deviceInfo);

    QTcpSocket*                  socket;
    QScopedPointer<QAudioSource> audioInput;
    QScopedPointer<QAudioSink>   audioOutput;
    QMediaDevices*               devices;
    QComboBox*                   inputDeviceBox;
    QComboBox*                   outputDeviceBox;
    QPushButton*                 speakBtn;
    QPushButton*                 quitBtn;
};

#endif // TALKIE_H
