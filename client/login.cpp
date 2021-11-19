#include "login.h"

Login::Login(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;

    QHBoxLayout* hLayout = new QHBoxLayout;

    url = new QLineEdit;
    url->setText("localhost");
    hLayout->addWidget(url, 1);
    port = new QSpinBox;
    port->setRange(1, USHRT_MAX);
    port->setValue(7777);
    hLayout->addWidget(port);
    layout->addLayout(hLayout);

    username = new QLineEdit;
    username->setText("admin");
    layout->addWidget(username);
    password = new QLineEdit;
    password->setText("admin");
    password->setEchoMode(QLineEdit::Password);
    layout->addWidget(password);
    code = new QLineEdit;
    code->setText("test");
    layout->addWidget(code);
    connectBtn = new QPushButton;
    connectBtn->setText("Connect");
    layout->addWidget(connectBtn);

    setLayout(layout);

    connect(connectBtn, &QPushButton::clicked, this, [=]() {
        emit connectToHost(url->text(), port->value(), username->text(), password->text(), code->text());
    });
}

void Login::setConnectBtnEnabled(bool enabled)
{
    connectBtn->setEnabled(enabled);
}
