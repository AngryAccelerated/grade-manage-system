#ifndef COUNSELLORINFOFORM_H
#define COUNSELLORINFOFORM_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>
namespace Ui {
class CounsellorInfoForm;
}

class CounsellorInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit CounsellorInfoForm(QWidget *parent = nullptr);
    ~CounsellorInfoForm();
    void closeEvent(QCloseEvent *event);
    void setInfo(const QString &id, const QString &name ,const  int& cl);


private slots:
    void on_btnConfirm_clicked();
    void deleteField();

private:
    Ui::CounsellorInfoForm *ui;
};

#endif // COUNSELLORINFOFORM_H
