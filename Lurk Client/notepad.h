#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include "qtimer.h"
#include <LURK.h>
namespace Ui {
class NotePad;
}

class NotePad : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotePad(QWidget *parent = 0);
    ~NotePad();


private slots:
    void on_actionConnect_triggered();

    void on_actionClose_triggered();
    void update();

    void on_players_clicked(const QModelIndex &index);

    void on_monsters_clicked(const QModelIndex &index);

    void on_LootSelect_activated(const QString &arg1);

    void on_LootSelect_currentIndexChanged(const QString &arg1);

    void on_RoomSelect_currentIndexChanged(int index);

    void on_FIght_clicked();

    void on_chatSend_2_clicked();

    void on_ChatInput_returnPressed();

    void on_joinFights_stateChanged(int arg1);

private:
    void died();
    void changeRoom(LURK::roomPkg roomIn);
    void msgIn();
    void updateChar(LURK::charPkg charIn);
    void updateChar();
    void errorIn();
    void roomIn();
    void charIn();
    QTimer *timer;
    Ui::NotePad *ui;


    void setEnabledControles(bool set);
};

#endif // NOTEPAD_H
