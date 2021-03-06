#ifndef CONNECTIONWIDOW_H
#define CONNECTIONWIDOW_H
#include <LURK.h>
#include <QDialog>

namespace Ui {
class ConnectionWidow;
}

class ConnectionWidow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionWidow(QWidget *parent = 0);
    ~ConnectionWidow();
    void accept();
    void content(LURK::gamePkg* game);
    Ui::ConnectionWidow *ui;
private slots:
    void on_actionUpdatePoint_triggered();
    void on_actionClose_triggered();
};

#endif // CONNECTIONWIDOW_H
