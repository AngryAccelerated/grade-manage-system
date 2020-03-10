#ifndef ADDACCOUNTFORM_H
#define ADDACCOUNTFORM_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>
namespace Ui {
class AddAccountForm;
}

class AddAccountForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddAccountForm(QWidget *parent = nullptr);
    ~AddAccountForm();
    void setInfo(const QString& id,const QString &pwd, const int &auth);

private slots:
    void on_pushButton_clicked();
private:
    Ui::AddAccountForm *ui;
};

#endif // ADDACCOUNTFORM_H
