#ifndef TEACHERFORM_H
#define TEACHERFORM_H

#include <QMainWindow>
#include <QtCharts>
#include <QtSql>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>
#include <QtXlsx>
namespace Ui {
class TeacherForm;
}

class TeacherForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeacherForm(QWidget *parent = nullptr);
    ~TeacherForm();

    QString getId() const;
    void setId(const QString &value);
    void closeEvent(QCloseEvent *event);
signals:
    void logout();

public slots:
    void initializeData();
private slots:
    void on_acLogout_triggered();
    void calculateAverage();
    void importXlsxFile();
    void on_cbbCourse_currentIndexChanged(const QString &arg1);
    void loadGradeTable();
    void on_cbbTerm_currentIndexChanged(int index);

    void on_acExport_triggered();

    void on_acImport_triggered();

    void on_acSave_triggered();

    void on_acStatistics_triggered();

    void on_actionDelete_triggered();

    void explodeHoveredSlice(bool show);

    const QString transferGradeToLevel(const int & grade);

    void tableDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void on_actionSave_All_triggered();


private:
    Ui::TeacherForm *ui;
    QString id;
    QMap<QString, int> mapCourse;
    QMap<QString, QString> mapCourseID;
    QMap<QString, int> mapCourseCredit;
    QList<QSqlQuery*> listQuery;
};

#endif // TEACHERFORM_H
