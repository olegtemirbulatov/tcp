#include <QApplication>

#include "tcpclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPClient w;
    w.show();
    return a.exec();
}
