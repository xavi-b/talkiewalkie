#include "server.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    bool   ok   = true;
    ushort port = a.arguments().size() > 1 ? a.arguments().at(1).toUShort(&ok) : 7777;
    if (!ok)
    {
        qWarning() << "Port argument must be a number between" << 1 << "and" << USHRT_MAX;
        return 1;
    }

    Server w(port);
    w.show();
    return a.exec();
}
