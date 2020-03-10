#ifndef STUDENTFORM_H
#define STUDENTFORM_H

#include <QMainWindow>
#include <QtCharts>
#include <QtSql>
#include <QDebug>
#include <QTableView>
#include <QMessageBox>
#include <QtXlsx>

namespace Ui {
class StudentForm;
}

class StudentForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentForm(QWidget *parent = nullptr);
    ~StudentForm();
    void queryStudentTotalGrade();
    void queryStudentBasicInfo();

    QString getId() const;
    void setId(const QString &value);
    void initializeData();

    void closeEvent(QCloseEvent *event);
    QString getFromWhere() const;
    void setFromWhere(const QString &value);

signals:
    void logout();

public slots:
    void plotGradeGraph();
    void plotGradeLineGraph();
    void saveGradeTable();
    void blurQuerySingleSubject();
private slots:
    void on_acExit_triggered();

private:
    Ui::StudentForm *ui;
    QString id;
    QString fromWhere;
};

#endif // STUDENTFORM_H
