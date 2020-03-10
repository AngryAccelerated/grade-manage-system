#include "querystudentgradeform.h"
#include "ui_querystudentgradeform.h"

QueryStudentGradeForm::QueryStudentGradeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryStudentGradeForm)
{
    ui->setupUi(this);
}

QueryStudentGradeForm::~QueryStudentGradeForm()
{
    delete ui;
}

void QueryStudentGradeForm::on_btnConfirm_clicked()
{
    emit confirmQueryRange(ui->sbMaximum->value(),ui->sbMinimum->value());
    this->close();
}
