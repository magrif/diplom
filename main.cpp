#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<VARIANT>("VARIANT");
    QApplication a(argc, argv);
    CDemoDlg w;
    w.show();

    return a.exec();
}
