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

class AudioInfo : public QIODevice
{
    Q_OBJECT

public:
    AudioInfo(const QAudioFormat& format);

    void start();
    void stop();

    qreal level() const
    {
        return m_level;
    }

    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;

private:
    const QAudioFormat m_format;
    qreal              m_level = 0.0; // 0.0 <= m_level <= 1.0

signals:
    void update();
};

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget* parent = nullptr);

    void setLevel(qreal value);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    qreal   m_level = 0;
    QPixmap m_pixmap;
};

class Talkie : public QWidget
{
    Q_OBJECT
public:
    explicit Talkie(QWidget* parent = nullptr);
    void record();
    void stop();
    void play(QByteArray const& data);

signals:
    void send(QByteArray const&);
    void quit();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void initializeInputAudio(const QAudioDevice& deviceInfo);
    void initializeOutputAudio(const QAudioDevice& deviceInfo);

    QScopedPointer<AudioInfo>    audioInfo;
    QScopedPointer<QAudioSource> audioInput;
    QScopedPointer<QAudioSink>   audioOutput;
    RenderArea*                  canvas;
    QMediaDevices*               devices;
    QComboBox*                   inputDeviceBox;
    QComboBox*                   outputDeviceBox;
    QPushButton*                 speakBtn;
    QPushButton*                 quitBtn;
};

#endif // TALKIE_H
