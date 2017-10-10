#ifndef ERROR_H
#define ERROR_H
#include <string.h>
#include <QDialog>

namespace Ui {
class error;
}

class error : public QDialog
{
    Q_OBJECT

public:
    void setError(std::string e);
    explicit error(QWidget *parent = 0);
    ~error();

private slots:
    void on_actionok_triggered();

private:
    std::string errorCode;
    Ui::error *ui;
};

#endif // ERROR_H
