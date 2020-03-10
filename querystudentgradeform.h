#ifndef QUERYSTUDENTGRADEFORM_H
#define QUERYSTUDENTGRADEFORM_H

#include <QWidget>

namespace Ui {
class QueryStudentGradeForm;
}

class QueryStudentGradeForm : public QWidget
{
    Q_OBJECT

public:
    explicit QueryStudentGradeForm(QWidget *parent = nullptr);
    ~QueryStudentGradeForm();
signals:
    void confirmQueryRange(const qreal& maximum,const qreal& minimum);
private slots:
    void on_btnConfirm_clicked();

private:
    Ui::QueryStudentGradeForm *ui;
};

#endif // QUERYSTUDENTGRADEFORM_H
