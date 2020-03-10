#include "courseinfoform.h"
#include "ui_courseinfoform.h"

CourseInfoForm::CourseInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseInfoForm)
{
    ui->setupUi(this);
}

CourseInfoForm::~CourseInfoForm()
{
    delete ui;
}

void CourseInfoForm::closeEvent(QCloseEvent *event)
{
    this->close();
}

void CourseInfoForm::setInfo(const QString &course_id, const QString &course_name, const QString &teacher_id, const int &period, const int &credit)
{
    ui->txtCourseID->setText(course_id);
    ui->txtTeacherID->setText(course_id);
    ui->txtCourseName->setText(course_name);
    ui->sbCredit->setValue(credit);
    ui->sbPeriod->setValue(period);
}

void CourseInfoForm::on_btnOK_clicked()
{
    if(!(ui->txtCourseID->text().isEmpty() && ui->txtTeacherID->text().isEmpty() && ui->txtCourseName->text().isEmpty()))
    {
        QSqlQuery query;
        query.exec(QString("replace into CourseInfo values ('%1','%2','%3',%4,%5)").arg(ui->txtCourseID->text()).arg(ui->txtCourseName->text()).arg(ui->txtTeacherID->text()).arg(ui->sbPeriod->value()).arg(ui->sbCredit->value()));
        QMessageBox msg;
        msg.setText(tr("Complete"));
        msg.exec();
        for(int i = 1;i <= 6;i++)
        {

            QSqlQuery createGrade(QString("create table Course_%1_Teacher_%2_Term_%3 (id TEXT not null PRIMARY KEY, grade INT)")
                                                    .arg(ui->txtCourseID->text())
                                                    .arg(ui->txtTeacherID->text())
                                                    .arg(i));
            QSqlQuery insertDefaultData(QString("insert into Course_%3_Teacher_%4_Term_%5 values ('%1',%2)").arg(0).arg(0)
                                                          .arg(ui->txtCourseID->text())
                                                          .arg(ui->txtTeacherID->text())
                                                          .arg(i));
            createGrade.exec();
            insertDefaultData.exec();
        }
    }
    else
    {
        QMessageBox msg;
        msg.setText(tr("Field cannot be empty"));
        msg.exec();
    }
}
