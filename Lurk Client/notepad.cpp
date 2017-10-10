#include "notepad.h"
#include "ui_notepad.h"
#include <string>
#include <stdlib.h>
#include <connectionwidow.h>
#include <server.h>
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
}

NotePad::~NotePad()
{
    delete ui;
}

void NotePad::on_actionConnect_triggered()
{
    Server s;

    s.setModal(true);
    s.exec();

}
