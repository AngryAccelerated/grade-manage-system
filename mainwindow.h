#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QtCharts>
#include <QTableWidget>
#include <QMessageBox>
#include "studentform.h"
#include "teacherform.h"
#include "counsellorform.h"
#include "studentinfoform.h"
#include "teacherinfoform.h"
#include "counsellorinfoform.h"
#include "addaccountform.h"
#include "courseinfoform.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    bool loginAccount(const QString &account, const QString &password);
    void closeEvent(QCloseEvent *event);

signals:
    void displayLoginForm();
public slots:
    void userExit();
    void displayWindow();
private slots:
    void on_actionQuit_triggered();

    void on_actionStudent_triggered();

    void on_actionTeacher_triggered();

    void on_actionCounsellor_triggered();

    void on_actionCourse_triggered();

    void on_actionAccount_triggered();

    void on_actionSave_triggered();

    void initializeData();

    void studentChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void teacherChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void counsellorChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void courseChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void accountChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);


private:
    Ui::MainWindow *ui;
    QString id;
    int auth;

    QList<QSqlQuery*> listQuery;
};
#endif // MAINWINDOW_H
