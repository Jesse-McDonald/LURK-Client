#ifndef SERVER_H
#define SERVER_H

#include <QDialog>

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    void accept();
private:
    Ui::Server *ui;
};

#endif // SERVER_H
