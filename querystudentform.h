#ifndef QUERYSTUDENTFORM_H
#define QUERYSTUDENTFORM_H

#include <QWidget>
#include <QtSql>
#include <counsellorform.h>
namespace Ui {
class QueryStudentForm;
}

class QueryStudentForm : public QWidget
{
    Q_OBJECT

public:
    explicit QueryStudentForm(QWidget *parent = nullptr);
    ~QueryStudentForm();
    QString getClassNumber() const;
    void setClassNumber(const QString &value);
    void initializeData();

signals:
    void confirmSingleStudent(const QString &id);
private slots:
    void on_txtName_textChanged(const QString &arg1);

    void on_listResult_doubleClicked(const QModelIndex &index);

private:
    Ui::QueryStudentForm *ui;
    QMap<QString,QString> mapStudent;

    QString classNumber;
    QStringListModel * model;
};

#endif // QUERYSTUDENTFORM_H
