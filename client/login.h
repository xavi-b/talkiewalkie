#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget* parent = nullptr);
    void setConnectBtnEnabled(bool enabled);

signals:
    void connectToHost(QString const&, int, QString const&, QString const&, QString const&);

private:
    QLineEdit*   url;
    QSpinBox*    port;
    QLineEdit*   username;
    QLineEdit*   password;
    QLineEdit*   code;
    QPushButton* connectBtn;
};

#endif // LOGIN_H
