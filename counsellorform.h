#ifndef COUNSELLORFORM_H
#define COUNSELLORFORM_H

#include <QMainWindow>
#include <QtCharts>
#include <QtSql>
#include <QDebug>
#include <QTableView>
#include <QMessageBox>
#include <QPieSeries>
#include <QPieSlice>
#include <QtXlsx>
#include "studentform.h"
#include "querystudentform.h"
#include "querystudentgradeform.h"
namespace Ui {
class CounsellorForm;
}

class CounsellorForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CounsellorForm(QWidget *parent = nullptr);
    ~CounsellorForm();
    QString getId() const;
    void setId(const QString &value);
    void closeEvent(QCloseEvent *event);

signals:
    void logout();

public slots:
    void initializeData();
    void queryClassGradeInfo();
    void displaySingleStudentInfo();
    void plotTotalClassGrade();
    void calculateAverageGrade();
    void saveClassGrade();
    void querySingleStudent();
    void queryStudentByGrade();
    void explodeHoveredSlice(bool show);
    void highlightSingleStudent(const QString &id);
    void highlightRangeStudent(const qreal &maximum,const qreal &minimum);
    double calculateTheTotalGrade(const QString &id);
    double evaluateCourseGrade(const double & course_grade);

    QString evaluateStudentGradeLevel(const double & total_grade);
private slots:
    void on_acLogout_triggered();

    void on_actionSort_By_ID_triggered();

private:
    Ui::CounsellorForm *ui;
    QString id;
    QString classNumber;

};

#endif // COUNSELLORFORM_H
