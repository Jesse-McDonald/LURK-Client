#include "characterdisplay.h"
#include "ui_characterdisplay.h"

CharacterDisplay::CharacterDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CharacterDisplay)
{
    ui->setupUi(this);
}
void CharacterDisplay::setContents(LURK::charPkg inPak){
   ui->Name->setText(QString::fromStdString(inPak.name));
   ui->Description->setText(QString::fromStdString(inPak.description));
   ui->Atk->setMaximum(inPak.atk+1);
   ui->Atk->setValue(inPak.atk);
   ui->Def->setMaximum(inPak.def+1);
   ui->Def->setValue(inPak.def);
   ui->Reg->setMaximum(inPak.reg+1);
   ui->Reg->setValue(inPak.reg);
   ui->Gold->setMaximum(inPak.gold+1);
   ui->Gold->setValue(inPak.gold);
   ui->Health->setMaximum(inPak.health+1);
   ui->Health->setValue(inPak.health);
   ui->FIght->setEnabled(inPak.flags[6]);
   //ui->FIght->setEnabled(inPak.flags[5]);
}
CharacterDisplay::~CharacterDisplay()
{
    delete ui;
}

void CharacterDisplay::on_FIght_clicked()
{
    lurk.pvp(ui->Name->text().toStdString());
    this->close();
}

void CharacterDisplay::on_Ok_clicked()
{
    this->close();
}
