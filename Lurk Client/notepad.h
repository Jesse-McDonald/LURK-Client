#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>

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

private:
    Ui::NotePad *ui;
};

#endif // NOTEPAD_H
