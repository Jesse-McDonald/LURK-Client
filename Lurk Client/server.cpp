#include "LURK.h"
#include "server.h"
#include "ui_server.h"
#include <connectionwidow.h>
#include <iostream>
#include <exception>
#include <error.h>


Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
}

Server::~Server()
{
    delete ui;
}
void Server::accept()
{
    if (lurk.connected){
        lurk.leave();
    }
    try{
        lurk.connect(ui->ipIn->text().toStdString(),ui->portIn->text().toStdString());

        lurk.startReader();
        bool connected=false;
        this->setDisabled(true);
        connected=lurk.timeout(11,1000);
        lurk.connected=connected;
        if(connected){
            lurk.lock();
            lurk.serverIp=ui->ipIn->text().toStdString();
            lurk.serverPort=ui->portIn->text().toStdString();
            LURK::pkg* conv=(lurk.cPak.get());
            LURK::gamePkg* conv2= dynamic_cast<LURK::gamePkg*>(conv);
            if(conv2){
                lurk.gameData = *(conv2);
            }
            this->close();
            ConnectionWidow c;
            c.setModal(true);
            c.content(&lurk.gameData);
            //lurk.cPak=&lurk.nulPak;
            lurk.unlock();
            c.exec();
        }else{
            error c;
            c.setModal(true);
            c.setError("No game recieved from host");
            c.exec();
            this->setEnabled(true);
            lurk.leave();
        }
    }catch(const std::exception e){
        std::cout<<e.what()<<"\n";
        error c;
        c.setModal(true);
        c.setError("Could not resolve host name");
        c.exec();

    }

}


