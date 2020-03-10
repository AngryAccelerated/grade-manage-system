#include "addaccountform.h"
#include "ui_addaccountform.h"

AddAccountForm::AddAccountForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddAccountForm)
{
    ui->setupUi(this);
    ui->cbbAuth->addItem(tr("Manager"));
    ui->cbbAuth->addItem(tr("Teacher"));
    ui->cbbAuth->addItem(tr("Counsellor"));
    ui->cbbAuth->addItem(tr("Student"));
    ui->cbbAuth->setCurrentIndex(0);
}

AddAccountForm::~AddAccountForm()
{
    delete ui;
}

void AddAccountForm::setInfo(const QString &id, const QString &pwd, const int &auth)
{
    ui->txtAccount->setText(id);
    ui->txtPassword->setText(id);
    ui->cbbAuth->setCurrentIndex(auth);
}

void AddAccountForm::on_pushButton_clicked()
{
    if(!ui->txtAccount->text().isEmpty())
    {

        QSqlQuery query;
        query.exec(QString("replace into UserAccount values (%1,'%2',%3)").arg(ui->txtAccount->text()).arg(ui->txtPassword->text()).arg(ui->cbbAuth->currentIndex()));
        QMessageBox msg;
        msg.setText(tr("Complete"));
        msg.exec();
    }
    else
    {
        QMessageBox msg;
        msg.setText(tr("Field cannot be empty"));
        msg.exec();
    }
}



