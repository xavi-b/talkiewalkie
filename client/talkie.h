#ifndef TALKIE_H
#define TALKIE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class Talkie : public QWidget
{
    Q_OBJECT
public:
    explicit Talkie(QWidget* parent = nullptr);
    void record();
    void send();

signals:
    void quit();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    QPushButton* speakBtn;
    QPushButton* quitBtn;
};

#endif // TALKIE_H
