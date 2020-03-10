#ifndef STUDENTINFOFORM_H
#define STUDENTINFOFORM_H

#include <QWidget>
#include <QMessageBox>
#include <QtSql>
namespace Ui {
class StudentInfoForm;
}

class StudentInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit StudentInfoForm(QWidget *parent = nullptr);
    ~StudentInfoForm();
    void closeEvent(QCloseEvent *event);
    void setInfo(const QString &id, const QString &name, const int & gender, const QString &faculty, const QString &major, const int &cl);

private slots:
    void on_pushButton_clicked();

private:
    Ui::StudentInfoForm *ui;
};

#endif // STUDENTINFOFORM_H
