
#include "notepad.h"
#include <QApplication>
#include "LURK.h"
#include <iostream>
LURK lurk=LURK();
int main(int argc, char *argv[])
{
    std::cout << "Starting program" << std::endl;
    QApplication a(argc, argv);
    std::cout << "qt initilised" << std::endl;
    NotePad w;
    std::cout << "w" << std::endl;
    w.show();
    std::cout << "w.show" << std::endl;
    int ret=a.exec();
    lurk.leave();
    return ret;
}
