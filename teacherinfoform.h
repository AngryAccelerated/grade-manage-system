#ifndef TEACHERINFOFORM_H
#define TEACHERINFOFORM_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>
namespace Ui {
class TeacherInfoForm;
}

class TeacherInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherInfoForm(QWidget *parent = nullptr);
    ~TeacherInfoForm();
    void closeEvent(QCloseEvent *event);
    void setInfo(const QString &id, const QString &name, const int &gender, const QString &faculty);

private slots:
    void on_btnConfirm_clicked();

private:
    Ui::TeacherInfoForm *ui;
};

#endif // TEACHERINFOFORM_H
