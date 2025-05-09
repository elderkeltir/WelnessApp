#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    // The MainWindow constructor hides the window,
    // so you can uncomment the next line if you wish to show it at startup.
    // w.show();

    return a.exec();
}
