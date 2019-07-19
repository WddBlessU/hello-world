#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    loginThread* log = new loginThread((void*)&w,&a);
    log->start();

    serverThread* server = new serverThread(&a);
    server->start();

    return a.exec();
}
