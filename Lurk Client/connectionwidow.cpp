#include "connectionwidow.h"
#include "ui_connectionwidow.h"
#include "LURK.h"
ConnectionWidow::ConnectionWidow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionWidow)
{
    ui->setupUi(this);

}

ConnectionWidow::~ConnectionWidow()
{
    delete ui;
}
void ConnectionWidow::accept(){

    this->close();


}
