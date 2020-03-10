#include "studentinfoform.h"
#include "ui_studentinfoform.h"

StudentInfoForm::StudentInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentInfoForm)
{
    ui->setupUi(this);
    ui->cbbGender->addItem(tr("Male"));
    ui->cbbGender->addItem(tr("Female"));
}

StudentInfoForm::~StudentInfoForm()
{
    delete ui;
}

void StudentInfoForm::closeEvent(QCloseEvent *event)
{
    this->close();
}

void StudentInfoForm::setInfo(const QString &id, const QString &name, const int &gender, const QString &faculty, const QString &major, const int &cl)
{
    ui->txtClass->setText(QString::number(cl));
    ui->txtMajor->setText(major);
    ui->txtFaculty->setText(faculty);
    ui->txtStudentID->setText(id);
    ui->txtStudentName->setText(name);
    ui->cbbGender->setCurrentIndex(gender - 1);
}

void StudentInfoForm::on_pushButton_clicked()
{
    if(!(ui->txtClass->text().isEmpty() && ui->txtMajor->text().isEmpty() && ui->txtFaculty->text().isEmpty() && ui->txtStudentID->text().isEmpty() && ui->txtStudentName->text().isEmpty()))
    {

        QSqlQuery query;
        query.exec(QString("replace into UserAccount values (%1,%2,%3)").arg(ui->txtStudentID->text()).arg(ui->txtStudentID->text()).arg(3));
        query.exec(QString("replace into StudentInfo values (%1,'%2',%3,'%4','%5',%6)").arg(ui->txtStudentID->text())
                   .arg(ui->txtStudentName->text()).arg(ui->cbbGender->currentIndex() + 1).arg(ui->txtFaculty->text()).arg(ui->txtMajor->text()).arg(ui->txtClass->text()));
        query.exec(QString("create table if not exists StudentCourseInfo_%1('course_id' TEXT, 'teacher_id' TEXT, 'term' INT, 'course_grade' INT)").arg(ui->txtStudentID->text()));
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
