
#include "notepad.h"
#include <QApplication>
#include "LURK.h"
LURK lurk=LURK();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NotePad w;
    w.show();
    int ret=a.exec();
    return ret;
}
