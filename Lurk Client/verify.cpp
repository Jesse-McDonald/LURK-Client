#include "verify.h"
#include "ui_verify.h"

Verify::Verify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Verify)
{

    ui->setupUi(this);
    ui->Prompt->setWordWrap(true);
}

Verify::~Verify()
{
    delete ui;
}
void Verify::setError(std::string msg){
    ui->Prompt->setText(QString::fromStdString(msg));
}
void Verify::on_actionGood_triggered(){
    good=true;
    this->close();
}

void Verify::on_actionBad_triggered()
{
    good=false;
    this->close();
}
