#include "error.h"
#include "ui_error.h"

error::error(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::error)
{
    ui->setupUi(this);
}
void error::setError(std::string e){
    this->errorCode=e;
    ui->errorBox->setText(QString::fromStdString(this->errorCode));
}
error::~error()
{
    delete ui;
}

void error::on_actionok_triggered()
{
      this->close();
}
