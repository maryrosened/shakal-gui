#include "shakalwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShakalWindow w;
    w.show();
    return a.exec();
}
