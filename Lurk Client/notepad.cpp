#include "notepad.h"
#include "ui_notepad.h"
#include <string>
#include <stdlib.h>
#include <connectionwidow.h>
#include <server.h>
#include <iostream>
#include <error.h>
#include <characterdisplay.h>

//NotePad.ui->ChatWindow->append() pushes messages to the chat window
//NotePad.ui->GameWindow->setText() sets a message to the game window
//NotePad.ui->roomName->setText() sets current room name
NotePad::NotePad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotePad)
{

    ui->setupUi(this);
    ui->roomName->setText("");
    ui->actionClose->setDisabled(true);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

void NotePad::update(){
    lurk.lock();
    if(!lurk.pks.empty()){//posibly make this a while, the mutex prevents new packets from
        //being added while you are removing them, so the queue will eventualy empty

    std::shared_ptr<LURK::pkg> temp=lurk.pks.front();
    lurk.pks.pop();
    LURK::pkg* cPak=temp.get();
    switch (cPak->type){
    std::cout<<"handling type "<<cPak->type;
    case 0://no package waiting
        break;//depercated, should no longer happen
    case 1:{//push message to feed
        LURK::msgPkg msg = *((LURK::msgPkg*)cPak);
        if (!strcmp(msg.recName,lurk.currentChar.name)){
            ui->ChatWindow->append(QString::fromStdString("<"+std::string(msg.sender)+ " to Me> "+msg.msg));
        }else{
            ui->ChatWindow->append(QString::fromStdString("<"+std::string(msg.sender)+"> "+msg.msg));
        }
        break;}//finished
    case 7:{ // throw or handle error
        LURK::errorPkg err=*((LURK::errorPkg*) cPak);
        error e;
        e.show();
        e.setModal(true);
        e.setError("Error "+std::to_string(err.code)+": "+err.msg);
        ui->ChatWindow->append("ERROR CODE "+QString::number(err.code)+": "+QString::fromStdString(err.msg));
        e.exec();
        break;}//finished
    case 8:
        //accept should never be handled by this function
        break;
    case 9:{//packets being handled by this are only for changing room
        LURK::roomPkg room=*((LURK::roomPkg*) cPak);
        changeRoom(room);
        break;}//finished
    case 10:{//handle characters
        LURK::charPkg chr=*((LURK::charPkg*) cPak);
        bool alive=chr.flags[7];
        bool monster=chr.flags[5];

        bool player=!strcmp(lurk.currentChar.name,chr.name)&&!monster;
        if(player){
            //std::cout<<"player updated";
            lurk.currentChar=chr;

            if(!alive){
                died();
            }
            updateChar();
        }else if(!alive){
                lurk.lootables.push_back(chr);
                ui->LootSelect->addItem(chr.name);
        }else if(monster){
            int x=lurk.checkMonster(chr);
            if(x>=0){
                lurk.monsters[x]=chr;
            }else{
                lurk.monsters.push_back(chr);
                ui->monsters->addItem(chr.name);
            }
        }else{
            int x=lurk.checkPlayer(chr);
            if(x>=0){
                lurk.players[x]=chr;
            }else{
                lurk.players.push_back(chr);
                ui->players->addItem(chr.name);
                ui->sendTo->addItem(chr.name);
            }
        }
        break;}//finished
    case 11:
        //should never be handled by this function
        break;
    case 12://I think this is thrown if the server leaves, not sure.
        //lurk.leave();
        lurk.leave();
        break;
    case 13:{//new room conneciton
        LURK::roomPkg room=*((LURK::roomPkg*) cPak);
        lurk.connectedRooms.push_back(room);
        ui->RoomSelect->addItem(QString::fromStdString(room.name));
        break;}
    default:
        break;
    }}
    lurk.unlock();
}

NotePad::~NotePad(){
    delete ui;
}

void NotePad::died(){
 //not sure what to do here
    //on_actionClose_triggered();
    ui->ChatWindow->append("You have died");
}

void NotePad::changeRoom(LURK::roomPkg roomIn){
    lurk.currentRoom=roomIn;
    ui->roomName->setText(QString::fromStdString(roomIn.name));
    ui->GameWindow->setText(QString::fromStdString(roomIn.description));
    lurk.connectedRooms.clear();
    lurk.monsters.clear();
    lurk.players.clear();
    lurk.lootables.clear();
    ui->RoomSelect->clear();
    ui->LootSelect->clear();
    ui->players->clear();
    ui->monsters->clear();
    ui->RoomSelect->addItem("Select a Room");
    ui->LootSelect->addItem("Lootables");
    ui->sendTo->clear();
    ui->sendTo->addItem("All");
}

void NotePad::updateChar(){
    ui->def->setMaximum(lurk.game.statMax);
    ui->atk->setMaximum(lurk.game.statMax);
    ui->reg->setMaximum(lurk.game.statMax);
    ui->def->setValue(lurk.currentChar.def);
    ui->atk->setValue(lurk.currentChar.atk);
    ui->reg->setValue(lurk.currentChar.reg);
    ui->gold->setText(QString::number(lurk.currentChar.gold));
    ui->health->setText(QString::number(lurk.currentChar.health));
    ui->name->setText(QString::fromStdString(std::string(lurk.currentChar.name)));
    ui->joinFights->setChecked(lurk.currentChar.flags[6]);

    ui->name->setToolTip(QString::fromStdString(lurk.currentChar.description));
}

void NotePad::on_actionConnect_triggered(){
    Server s;
    s.setModal(true);
    s.exec();
    setEnabledControles(lurk.connected);
    if(lurk.connected){
        updateChar();
    }
}

void NotePad::on_actionClose_triggered(){
    lurk.leave();
    setEnabledControles(lurk.connected);
}

void NotePad::setEnabledControles(bool set){
    ui->def->setEnabled(set);
    ui->reg->setEnabled(set);
    ui->atk->setEnabled(set);
    ui->actionClose->setEnabled(set);
    ui->players->setEnabled(set);
    ui->monsters->setEnabled(set);
    ui->LootSelect->setEnabled(set);
    ui->LootSelect->clear();
    lurk.lootables.clear();
    lurk.connectedRooms.clear();
    ui->RoomSelect->clear();
    ui->RoomSelect->setEnabled(set);
    ui->roomName->clear();
    ui->name->clear();
    ui->GameWindow->clear();
    ui->ChatWindow->clear();
    ui->chatSend->setEnabled(set);
    ui->chatSend_2->setEnabled(set);
    ui->ChatInput->setEnabled(set);
    ui->RoomSelect->addItem("Select a Room");
    ui->LootSelect->addItem("Lootables");
    ui->joinFights->setEnabled(set);
    ui->sendTo->setEnabled(set);
}

void NotePad::on_players_clicked(const QModelIndex &index)
{
    CharacterDisplay c;
    c.setModal(true);
    c.setContents(lurk.players[index.row()]);
    c.exec();
}

void NotePad::on_monsters_clicked(const QModelIndex &index)
{
    CharacterDisplay c;
    c.setModal(true);
    c.setContents(lurk.monsters[index.row()]);
    c.exec();
}

void NotePad::on_LootSelect_currentIndexChanged(const QString &arg1)
{
    if(ui->LootSelect->currentIndex()>0){
        lurk.loot(arg1.toStdString());
        ui->LootSelect->setCurrentIndex(0);
    }
}

void NotePad::on_LootSelect_activated(const QString &arg1){}//ignore this, I accidentaly used this slot and now qt wants to keep using it

void NotePad::on_RoomSelect_currentIndexChanged(int index)
{

    if(index>0){
        LURK::roomPkg room = lurk.connectedRooms[index-1];
        lurk.chngrm(room.number);

        ui->LootSelect->setCurrentIndex(0);

    }
}

void NotePad::on_FIght_clicked()
{
    lurk.fight();
}

void NotePad::on_chatSend_2_clicked()
{
    std::string input=ui->ChatInput->text().toStdString();
    if(ui->sendTo->currentIndex()>0){
        ui->ChatWindow->append("<me to "+ui->sendTo->currentText()+"> "+ui->ChatInput->text());
        lurk.msg(ui->sendTo->currentText().toStdString(), input);
    }else{
        ui->ChatWindow->append(QString::fromStdString((std::string)"<"+lurk.currentChar.name+">")+ui->ChatInput->text());
        lurk.msg(input);
    }
    ui->ChatInput->clear();
}

void NotePad::on_ChatInput_returnPressed()
{
    on_chatSend_2_clicked();
}


void NotePad::on_joinFights_stateChanged(int arg1)
{
    lurk.lock();
    lurk.currentChar.flags[6]=arg1;
    lurk.character(lurk.currentChar);
    lurk.unlock();
}
