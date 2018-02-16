#ifndef CHARACTERDISPLAY_H
#define CHARACTERDISPLAY_H

#include <QDialog>
#include <LURK.h>
namespace Ui {
class CharacterDisplay;
}

class CharacterDisplay : public QDialog
{
    Q_OBJECT

public:
    void setContents(LURK::charPkg inPak);
    explicit CharacterDisplay(QWidget *parent = 0);
    ~CharacterDisplay();


private slots:
    void on_FIght_clicked();

    void on_Ok_clicked();

private:

    Ui::CharacterDisplay *ui;
};

#endif // CHARACTERDISPLAY_H
