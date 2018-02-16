#ifndef VERIFY_H
#define VERIFY_H

#include <QDialog>
#include <string>
namespace Ui {
class Verify;
}

class Verify : public QDialog
{
    Q_OBJECT

public:
    bool good;
    void setError(std::string msg);
    explicit Verify(QWidget *parent = 0);
    ~Verify();

private slots:
    void on_actionGood_triggered();

    void on_actionBad_triggered();

private:
    Ui::Verify *ui;
};

#endif // VERIFY_H
