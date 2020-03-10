#include "teacherinfoform.h"
#include "ui_teacherinfoform.h"

TeacherInfoForm::TeacherInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherInfoForm)
{
    ui->setupUi(this);
    ui->cbbGender->addItem(tr("Male"));
    ui->cbbGender->addItem(tr("Female"));
}

TeacherInfoForm::~TeacherInfoForm()
{
    delete ui;
}

void TeacherInfoForm::closeEvent(QCloseEvent *event)
{
    this->close();
}

void TeacherInfoForm::setInfo(const QString &id, const QString &name, const int &gender, const QString &faculty)
{
    ui->txtFaculty->setText(faculty);
    ui->txtTeacherID->setText(id);
    ui->txtTeacherName->setText(name);
    ui->cbbGender->setCurrentIndex(gender - 1);
}

void TeacherInfoForm::on_btnConfirm_clicked()
{
    if(!(ui->txtFaculty->text().isEmpty() && ui->txtTeacherID->text().isEmpty() && ui->txtTeacherName->text().isEmpty()))
    {
        QSqlQuery query;
        query.exec(QString("replace into TeacherInfo values (%1,'%2','%3',%4)").arg(ui->txtTeacherID->text()).arg(ui->txtTeacherName->text()).arg(ui->txtFaculty->text()).arg(ui->cbbGender->currentIndex() + 1));
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
