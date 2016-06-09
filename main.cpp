#include "desktop.h"
#include <QApplication>
#include<QApplication>
#include<QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    desktop w;
    QString name=argv[1];
    if(argc==2) w.setname(name);
    w.show();
    w.move(QApplication::desktop()->width()/2-w.width()/2,QApplication::desktop()->height()/2-w.height()/2);
    return a.exec();
}
