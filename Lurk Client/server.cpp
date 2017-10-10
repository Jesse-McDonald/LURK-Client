#include "LURK.h"
#include "server.h"
#include "ui_server.h"
#include <connectionwidow.h>
#include <iostream>
#include <exception>
#include <error.h>
#include <thread>
#include <chrono>

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
    try{
        lurk.connect(ui->ipIn->text().toStdString(),ui->portIn->text().toStdString());

        lurk.startReader();
        bool connected=false;
        this->setDisabled(true);
        for(int i=0;i<1000;i++){
            if(lurk.cPak->type==11){
                connected=true;
                break;
            }
             std::this_thread::sleep_for (std::chrono::milliseconds(1));
        }
        if(connected){
            this->close();
            ConnectionWidow c;
            c.setModal(true);
            c.exec();
        }else{
            error c;
            c.setModal(true);
            c.setError("No game recieved from host");
            c.exec();
            this->setEnabled(true);
        }
    }catch(const std::exception e){
        error c;
        c.setModal(true);
        c.setError("Could not resolve host name");
        c.exec();

    }

}


