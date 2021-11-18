#include "talkie.h"

Talkie::Talkie(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;

    speakBtn = new QPushButton;
    speakBtn->setText("Speak");
    layout->addWidget(speakBtn);
    quitBtn = new QPushButton;
    quitBtn->setText("Quit");
    layout->addWidget(quitBtn);

    setLayout(layout);

    connect(quitBtn, &QPushButton::clicked, this, &Talkie::quit);

    connect(speakBtn, &QPushButton::pressed, this, &Talkie::record);
    connect(speakBtn, &QPushButton::released, this, &Talkie::send);

    //    QAudioFormat format;
    //    format.setChannelCount(1);
    //    format.setSampleRate(8000);
    //    format.setSampleSize(8);
    //    format.setCodec("audio/pcm");
    //    format.setByteOrder(QAudioFormat::LittleEndian);
    //    format.setSampleType(QAudioFormat::UnSignedInt);
    // https://stackoverflow.com/questions/25498510/qt-stream-audio-over-tcp-socket-in-realtime
}

void Talkie::record()
{
    //TODO
}

void Talkie::send()
{
    //TODO
}

void Talkie::keyPressEvent(QKeyEvent* event)
{
    record();
    QWidget::keyPressEvent(event);
}

void Talkie::keyReleaseEvent(QKeyEvent* event)
{
    send();
    QWidget::keyReleaseEvent(event);
}
