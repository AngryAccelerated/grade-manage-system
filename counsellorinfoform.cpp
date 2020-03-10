#include "counsellorinfoform.h"
#include "ui_counsellorinfoform.h"

CounsellorInfoForm::CounsellorInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CounsellorInfoForm)
{
    ui->setupUi(this);
}

CounsellorInfoForm::~CounsellorInfoForm()
{
    delete ui;
}

void CounsellorInfoForm::closeEvent(QCloseEvent *event)
{
    this->close();
}

void CounsellorInfoForm::setInfo(const QString &id, const QString &name, const int &cl)
{
    ui->txtCounsellorID->setText(id);
    ui->txtCounsellorName->setText(name);
    ui->txtCounsellorClass->setText(QString::number(cl));
}

void CounsellorInfoForm::on_btnConfirm_clicked()
{
    if(!(ui->txtCounsellorID->text().isEmpty() && ui->txtCounsellorName->text().isEmpty() && ui->txtCounsellorClass->text().isEmpty()))
    {

        QSqlQuery query;
        query.exec(QString("replace into ClassInfo values (%1,'%2','%3')").arg(ui->txtCounsellorClass->text()).arg(ui->txtCounsellorID->text()).arg(ui->txtCounsellorName->text()));
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

void CounsellorInfoForm::deleteField()
{

}

