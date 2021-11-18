#include "server.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    if (a.arguments().size() < 1)
    {
        qWarning() << "Missing port argument";
        return 1;
    }

    bool   ok   = false;
    ushort port = a.arguments().at(1).toUShort(&ok);
    if (!ok)
    {
        qWarning() << "Port argument must be a number between" << 1 << "and" << USHRT_MAX;
        return 1;
    }

    Server w(port);
    w.show();
    return a.exec();
}
