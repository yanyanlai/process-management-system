#include "mainwindow.h"

#include <QApplication>
#include "dispatcher.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    dispatcher d;
//    d.add(3, 1);
//    d.add(3, 2);
//    d.run(3);


    return a.exec();
}
