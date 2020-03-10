#ifndef COURSEINFOFORM_H
#define COURSEINFOFORM_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>
namespace Ui {
class CourseInfoForm;
}

class CourseInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit CourseInfoForm(QWidget *parent = nullptr);
    ~CourseInfoForm();
    void closeEvent(QCloseEvent *event);
    void setInfo(const QString& course_id,const QString& course_name,const QString& teacher_id,const int &period, const int &credit);
private slots:
    void on_btnOK_clicked();

private:
    Ui::CourseInfoForm *ui;
};

#endif // COURSEINFOFORM_H
