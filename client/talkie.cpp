#include "talkie.h"

Talkie::Talkie(QTcpSocket* socket, QWidget* parent)
    : QWidget(parent),
      socket(socket),
      devices(new QMediaDevices(this))
{
    QVBoxLayout* layout = new QVBoxLayout;

    inputDeviceBox                             = new QComboBox(this);
    const QAudioDevice& defaultInputDeviceInfo = devices->defaultAudioInput();
    inputDeviceBox->addItem(defaultInputDeviceInfo.description(), QVariant::fromValue(defaultInputDeviceInfo));
    for (auto& deviceInfo : devices->audioInputs())
    {
        if (deviceInfo != defaultInputDeviceInfo)
            inputDeviceBox->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
    }
    layout->addWidget(inputDeviceBox);

    outputDeviceBox                             = new QComboBox(this);
    const QAudioDevice& defaultOutputDeviceInfo = devices->defaultAudioOutput();
    outputDeviceBox->addItem(defaultOutputDeviceInfo.description(), QVariant::fromValue(defaultOutputDeviceInfo));
    for (auto& deviceInfo : devices->audioOutputs())
    {
        if (deviceInfo != defaultOutputDeviceInfo)
            outputDeviceBox->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
    }
    layout->addWidget(outputDeviceBox);

    speakBtn = new QPushButton;
    speakBtn->setText("Speak");
    layout->addWidget(speakBtn);
    quitBtn = new QPushButton;
    quitBtn->setText("Quit");
    layout->addWidget(quitBtn);

    setLayout(layout);

    connect(inputDeviceBox, QOverload<int>::of(&QComboBox::activated), this, [=](int index) {
        audioInput->stop();
        audioInput->disconnect(this);

        initializeInputAudio(inputDeviceBox->itemData(index).value<QAudioDevice>());
    });

    connect(outputDeviceBox, QOverload<int>::of(&QComboBox::activated), this, [=](int index) {
        audioOutput->stop();
        audioOutput->disconnect(this);

        initializeOutputAudio(outputDeviceBox->itemData(index).value<QAudioDevice>());
    });

    connect(quitBtn, &QPushButton::clicked, this, &Talkie::quit);

    connect(speakBtn, &QPushButton::pressed, this, &Talkie::record);
    connect(speakBtn, &QPushButton::released, this, &Talkie::stop);

    initializeInputAudio(devices->defaultAudioInput());
    initializeOutputAudio(devices->defaultAudioOutput());
}

void Talkie::initializeInputAudio(const QAudioDevice& deviceInfo)
{
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    audioInput.reset(new QAudioSource(deviceInfo, format));

    connect(audioInput.get(), &QAudioSource::stateChanged, this, [=](QAudio::State state) {
        qDebug() << "QAudioSource" << state;
    });
}

void Talkie::initializeOutputAudio(const QAudioDevice& deviceInfo)
{
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    audioOutput.reset(new QAudioSink(deviceInfo, format));
    audioOutput->setVolume(2.0f);
    audioOutput->start(socket);

    qDebug() << "QAudioSink" << audioOutput->state();

    connect(audioOutput.get(), &QAudioSink::stateChanged, this, [=](QAudio::State state) {
        qDebug() << "QAudioSink" << state;
    });
}

void Talkie::record()
{
    qDebug() << "Record";

    //send("BEEP");
    audioInput->start(socket);
}

void Talkie::stop()
{
    qDebug() << "Stop";

    audioInput->stop();
    //send("BEEP");
}

void Talkie::play()
{
    qDebug() << "Data";
    //audioOutput->resume();
}

void Talkie::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_R)
    {
        event->setAccepted(true);
        record();
    }
    else
        QWidget::keyPressEvent(event);
}

void Talkie::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_R)
    {
        event->setAccepted(true);
        stop();
    }
    else
        QWidget::keyReleaseEvent(event);
}
