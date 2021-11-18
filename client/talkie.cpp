#include "talkie.h"

AudioInfo::AudioInfo(const QAudioFormat& format)
    : m_format(format)
{
}

void AudioInfo::start()
{
    open(QIODevice::WriteOnly);
}

void AudioInfo::stop()
{
    close();
}

qint64 AudioInfo::readData(char* data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);

    return 0;
}

qint64 AudioInfo::writeData(const char* data, qint64 len)
{
    const int channelBytes = m_format.bytesPerSample();
    const int sampleBytes  = m_format.bytesPerFrame();
    Q_ASSERT(len % sampleBytes == 0);
    const int numSamples = len / sampleBytes;

    float                maxValue = 0;
    const unsigned char* ptr      = reinterpret_cast<const unsigned char*>(data);

    for (int i = 0; i < numSamples; ++i)
    {
        for (int j = 0; j < m_format.channelCount(); ++j)
        {
            float value = m_format.normalizedSampleValue(ptr);

            maxValue = qMax(value, maxValue);
            ptr += channelBytes;
        }
    }

    m_level = maxValue;

    emit update();
    return len;
}

RenderArea::RenderArea(QWidget* parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    setMinimumHeight(30);
    setMinimumWidth(200);
}

void RenderArea::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.drawRect(QRect(painter.viewport().left() + 10,
                           painter.viewport().top() + 10,
                           painter.viewport().right() - 20,
                           painter.viewport().bottom() - 20));
    if (m_level == 0.0)
        return;

    int pos = ((painter.viewport().right() - 20) - (painter.viewport().left() + 11)) * m_level;
    painter.fillRect(painter.viewport().left() + 11,
                     painter.viewport().top() + 10,
                     pos,
                     painter.viewport().height() - 21,
                     Qt::red);
}

void RenderArea::setLevel(qreal value)
{
    m_level = value;
    update();
}

Talkie::Talkie(QWidget* parent)
    : QWidget(parent),
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

    canvas = new RenderArea(this);
    layout->addWidget(canvas);

    speakBtn = new QPushButton;
    speakBtn->setText("Speak");
    layout->addWidget(speakBtn);
    quitBtn = new QPushButton;
    quitBtn->setText("Quit");
    layout->addWidget(quitBtn);

    setLayout(layout);

    connect(inputDeviceBox, QOverload<int>::of(&QComboBox::activated), this, [=](int index) {
        audioInfo->stop();
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

    audioInfo.reset(new AudioInfo(format));
    connect(audioInfo.data(), &AudioInfo::update, [this]() {
        canvas->setLevel(audioInfo->level());
    });

    audioInput.reset(new QAudioSource(deviceInfo, format));
    audioInfo->start();
}

void Talkie::initializeOutputAudio(const QAudioDevice& deviceInfo)
{
    QAudioFormat format = deviceInfo.preferredFormat();
    audioOutput.reset(new QAudioSink(deviceInfo, format));
}

void Talkie::record()
{
    send("BEEP");
    auto io = audioInput->start();
    connect(io, &QIODevice::readyRead, [&, io]() {
        qint64    len        = audioInput->bytesAvailable();
        const int BufferSize = 4096;
        if (len > BufferSize)
            len = BufferSize;

        QByteArray buffer(len, 0);
        qint64     l = io->read(buffer.data(), len);
        if (l > 0)
        {
            audioInfo->write(buffer.constData(), l);
            send(QByteArray(buffer.constData(), l));
        }
    });
}

void Talkie::stop()
{
    audioInput->suspend();
    send("BEEP");
}

void Talkie::play(QByteArray const& data)
{
    auto io = audioOutput->start();
    if (audioOutput->state() == QAudio::StoppedState)
        return;
    io->write(data);
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
