#include "connectionwidow.h"
#include "ui_connectionwidow.h"
#include "LURK.h"
#include "error.h"
#include <verify.h>
#include <algorithm>
#include <iostream>
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
void ConnectionWidow::content(LURK::gamePkg* game){
       ui->gameDesc->setText(QString::fromStdString(game->description));
       ui->ip->setText(QString::fromStdString(lurk.serverIp));
       ui->port->setText(QString::fromStdString(lurk.serverPort));
       ui->initPoints->setText(QString::number(game->init));
       lurk.game=*game;
}
void ConnectionWidow::accept(){
    if(ui->NameIn->text()==""){
        error c;
        c.setModal(true);
        c.setError("Please enter a name.");
        c.exec();
        return;
    }

    if (0!=lurk.gameData.init-(ui->atkIn->value()+ui->defIn->value()+ui->RegenIn->value())){
        Verify c;
        c.setModal(true);
        c.setError("You still have points remaining are you sure you want to continue?");

        c.exec();
        if (!c.good) return;
    }
    if (ui->cDescriptionIn->toPlainText()==""){
        Verify c;
        c.setModal(true);
        c.setError("You did not enter a character description, are you sure you want to continue?");

        c.exec();
        if (!c.good) return;
    }
    LURK::charPkg c;
    c.atk=ui->atkIn->text().toInt();
    c.def=ui->defIn->text().toInt();
    c.reg=ui->RegenIn->text().toInt();
    c.description=ui->cDescriptionIn->toPlainText().toStdString();
    for (int i=0;i<32;i++) c.name[i]=ui->NameIn->text().toStdString().c_str()[i];
    c.flags[6]=ui->joinFight->isChecked();
    c.type=10;
    c.gold=0;
    c.health=0;
    c.room=0;
    lurk.character(c);
    bool connected=false;
    connected =lurk.timeout(8,1000);
    connected=true;
    lurk.lock();
    if(lurk.pks.front()->type==7){

         error e;
         LURK::pkg* conv=(lurk.pks.front().get());
         LURK::errorPkg* conv2= dynamic_cast<LURK::errorPkg*>(conv);
         LURK::errorPkg errorMsg=*conv2;

         e.setModal(true);
         e.setError("Error recieved from server:\n\""+errorMsg.msg+"\"");
         lurk.pks.pop();
         e.exec();
    }else if(connected){
        lurk.currentChar=c;
        for (int i=0;i<32;i++) lurk.currentChar.name[i]=c.name[i];
        lurk.start();

        this->close();
    }else{
        error c;
        c.setModal(true);
        c.setError("No acceptance recieved from server");
        c.exec();
    }
    lurk.unlock();

}

void ConnectionWidow::on_actionUpdatePoint_triggered()
{
    ui->initPoints->setText(QString::number(lurk.gameData.init-(ui->atkIn->value()+ui->defIn->value()+ui->RegenIn->value())));
    ui->atkIn->setMaximum(std::min(ui->atkIn->value()+ui->initPoints->text().toInt(),(int)lurk.game.statMax));
    ui->defIn->setMaximum(std::min(ui->defIn->value()+ui->initPoints->text().toInt(),(int)lurk.game.statMax));
    ui->RegenIn->setMaximum(std::min(ui->RegenIn->value()+ui->initPoints->text().toInt(),(int)lurk.game.statMax));
}

void ConnectionWidow::on_actionClose_triggered()
{
    lurk.leave();
    this->close();
}
