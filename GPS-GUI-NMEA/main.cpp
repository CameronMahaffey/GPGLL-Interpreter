#include "mywindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWindow w;
    w.setFixedSize(665, 350);
    w.setWindowTitle("GPS Position");

    w.show();
    return a.exec();
}
