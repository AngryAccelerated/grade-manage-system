#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loginAccount(const QString &account, const QString &password)
{
    QSqlQuery query;
    query.exec(QString("select password,auth from UserAccount where id = '%1'").arg(account));
    while(query.next())
    {
        QString pwd = query.value(0).toString();
        if(pwd == password){
            id = account;
            auth = query.value(1).toInt();
            return true;
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int r = QMessageBox::question(this,tr("Last Hint"),tr("Are you sure you want to quit?"),QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (r==QMessageBox::Yes){
        event->accept();
        userExit();
    }
    else
        event->ignore();
}

void MainWindow::userExit()
{
    emit displayLoginForm();
    qDebug() << "exit!";
}

void MainWindow::displayWindow()
{
    if(auth == 0)
    {

        this->show();
        initializeData();
        qDebug() << "auth";
    }
    else if(auth == 1)
    {

        qDebug() << "teacher";
        TeacherForm * frmTeacher = new TeacherForm();
        connect(frmTeacher,&TeacherForm::logout,this,&MainWindow::userExit);
        frmTeacher->setId(id);
        frmTeacher->initializeData();
        frmTeacher->show();
    }
    else if(auth == 2)
    {

        qDebug() << "counselor";
        CounsellorForm * frmCounsellor = new CounsellorForm();
        connect(frmCounsellor,&CounsellorForm::logout,this,&MainWindow::userExit);
        frmCounsellor->setId(id);
        frmCounsellor->initializeData();
        frmCounsellor->show();
    }
    else if(auth == 3)
    {

        qDebug() << "student";
        StudentForm * frmStudent = new StudentForm();
        connect(frmStudent,&StudentForm::logout,this,&MainWindow::userExit);
        frmStudent->setId(id);
        frmStudent->setFromWhere("Login");
        frmStudent->initializeData();
        frmStudent->show();
    }
}


void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionStudent_triggered()
{
    StudentInfoForm * frm = new StudentInfoForm();
    frm->show();
}

void MainWindow::on_actionTeacher_triggered()
{
    TeacherInfoForm * frm = new TeacherInfoForm();
    frm->show();
}

void MainWindow::on_actionCounsellor_triggered()
{
    CounsellorInfoForm * frm = new CounsellorInfoForm();
    frm->show();
}

void MainWindow::on_actionCourse_triggered()
{
    CourseInfoForm * frm = new CourseInfoForm();
    frm->show();
}

void MainWindow::initializeData()
{
    //Student
    QSqlQuery query;
    int j = 0;
    QStandardItemModel* model1 = new QStandardItemModel();
    QStandardItemModel* model2 = new QStandardItemModel();
    QStandardItemModel* model3 = new QStandardItemModel();
    QStandardItemModel* model4 = new QStandardItemModel();
    QStandardItemModel* model5 = new QStandardItemModel();
    ui->tableStudent->setModel(model1);
    ui->tableCounsellor->setModel(model2);
    ui->tableTeacher->setModel(model3);
    ui->tableCourse->setModel(model4);
    ui->tableAccount->setModel(model5);
    QStringList labels = QString(tr("Student ID,Name,Gender,Faculty,Major,Class")).split(",");
    model1->setHorizontalHeaderLabels(labels);
    query.exec(QString("select * from StudentInfo"));
    while(query.next())
    {
        QStandardItem * primaryKey = new QStandardItem(query.value(0).toString());
        primaryKey->setFlags(primaryKey->flags() & ~Qt::ItemIsEditable);
        model1->setItem(j,0, primaryKey);
        model1->setItem(j,1,new QStandardItem(query.value(1).toString()));
        model1->setItem(j,2,new QStandardItem(query.value(2).toString()));
        model1->setItem(j,3,new QStandardItem(query.value(3).toString()));
        model1->setItem(j,4,new QStandardItem(query.value(4).toString()));
        model1->setItem(j,5,new QStandardItem(query.value(5).toString()));
        j++;
    }
    //Counsellor
    j = 0;
    labels = QString(tr("Class,Counsellor ID,Name")).split(",");
    model2->setHorizontalHeaderLabels(labels);
    query.exec(QString("select * from ClassInfo"));
    while(query.next())
    {
        QStandardItem * primaryKey = new QStandardItem(query.value(0).toString());
        primaryKey->setFlags(primaryKey->flags() & ~Qt::ItemIsEditable);
        model2->setItem(j,0, primaryKey);
        model2->setItem(j,1,new QStandardItem(query.value(1).toString()));
        model2->setItem(j,2,new QStandardItem(query.value(2).toString()));
        j++;
    }
    //Teacher
    j = 0;
    labels = QString(tr("Teacher ID,Name,Faculty,Gender")).split(",");
    model3->setHorizontalHeaderLabels(labels);
    query.exec(QString("select * from TeacherInfo"));
    while(query.next())
    {
        QStandardItem * primaryKey = new QStandardItem(query.value(0).toString());
        primaryKey->setFlags(primaryKey->flags() & ~Qt::ItemIsEditable);
        model3->setItem(j,0, primaryKey);
        model3->setItem(j,1,new QStandardItem(query.value(1).toString()));
        model3->setItem(j,2,new QStandardItem(query.value(2).toString()));
        model3->setItem(j,3,new QStandardItem(query.value(3).toString()));
        j++;
    }
    //Course
    j = 0;
    labels = QString(tr("Course ID,Name,Teacher ID,Period,Credit")).split(",");
    model4->setHorizontalHeaderLabels(labels);
    query.exec(QString("select * from CourseInfo"));
    while(query.next())
    {
        QStandardItem * primaryKey = new QStandardItem(query.value(0).toString());
        primaryKey->setFlags(primaryKey->flags() & ~Qt::ItemIsEditable);
        model4->setItem(j,0, primaryKey);
        model4->setItem(j,1,new QStandardItem(query.value(1).toString()));
        model4->setItem(j,2,new QStandardItem(query.value(2).toString()));
        model4->setItem(j,3,new QStandardItem(query.value(3).toString()));
        model4->setItem(j,4,new QStandardItem(query.value(4).toString()));
        j++;
    }
    //Account
    j = 0;
    labels = QString(tr("Account,Password,Auth")).split(",");
    model5->setHorizontalHeaderLabels(labels);
    query.exec(QString("select * from UserAccount"));
    QString auth;
    while(query.next())
    {
        QStandardItem * primaryKey = new QStandardItem(query.value(0).toString());
        primaryKey->setFlags(primaryKey->flags() & ~Qt::ItemIsEditable);
        model5->setItem(j,0, primaryKey);
        model5->setItem(j,1,new QStandardItem(query.value(1).toString()));
        model5->setItem(j,2,new QStandardItem(query.value(2).toString()));
        j++;
    }

    connect(model1,&QStandardItemModel::dataChanged,this,&MainWindow::studentChanged);
    connect(model2,&QStandardItemModel::dataChanged,this,&MainWindow::counsellorChanged);
    connect(model3,&QStandardItemModel::dataChanged,this,&MainWindow::teacherChanged);
    connect(model4,&QStandardItemModel::dataChanged,this,&MainWindow::courseChanged);
    connect(model5,&QStandardItemModel::dataChanged,this,&MainWindow::accountChanged);

}

void MainWindow::on_actionSave_triggered()
{
    qDebug() << "save query!";
    foreach (QSqlQuery * query, listQuery) {
        query->exec();
        delete query;
    }
}

void MainWindow::studentChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    qDebug() << "confirm!";
    int row = topLeft.row();
    QSqlQuery * query = new QSqlQuery(QString("replace into StudentInfo values(%1,'%2',%3,'%4','%5',%6)")
                                          .arg(ui->tableStudent->model()->index(row,0).data().toString())
                                          .arg(ui->tableStudent->model()->index(row,1).data().toString())
                                          .arg(ui->tableStudent->model()->index(row,2).data().toString())
                                          .arg(ui->tableStudent->model()->index(row,3).data().toString())
                                          .arg(ui->tableStudent->model()->index(row,4).data().toString())
                                          .arg(ui->tableStudent->model()->index(row,5).data().toString()));
    listQuery.append(query);
}

void MainWindow::teacherChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    qDebug() << "confirm!";
    int row = topLeft.row();
    QSqlQuery * query = new QSqlQuery(QString("replace into TeacherInfo values('%1','%2','%3')")
                                          .arg(ui->tableTeacher->model()->index(row,0).data().toString())
                                          .arg(ui->tableTeacher->model()->index(row,1).data().toString())
                                          .arg(ui->tableTeacher->model()->index(row,2).data().toString())
                                          .arg(ui->tableTeacher->model()->index(row,3).data().toString()));
    listQuery.append(query);
}

void MainWindow::counsellorChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    qDebug() << "confirm!";
    int row = topLeft.row();
    QSqlQuery * query = new QSqlQuery(QString("replace into ClassInfo values(%1,'%2','%3',%4)")
                                          .arg(ui->tableCounsellor->model()->index(row,0).data().toString())
                                          .arg(ui->tableCounsellor->model()->index(row,1).data().toString())
                                          .arg(ui->tableCounsellor->model()->index(row,2).data().toString()));
    listQuery.append(query);
}

void MainWindow::courseChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    qDebug() << "confirm!";
    int row = topLeft.row();
    QSqlQuery * query = new QSqlQuery(QString("replace into CourseInfo values('%1','%2','%3',%4,%5)")
                                          .arg(ui->tableCourse->model()->index(row,0).data().toString())
                                          .arg(ui->tableCourse->model()->index(row,1).data().toString())
                                          .arg(ui->tableCourse->model()->index(row,2).data().toString())
                                          .arg(ui->tableCourse->model()->index(row,3).data().toString())
                                          .arg(ui->tableCourse->model()->index(row,4).data().toString()));
    listQuery.append(query);
}

void MainWindow::accountChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    qDebug() << "confirm!";
    int row = topLeft.row();
    QSqlQuery * query = new QSqlQuery(QString("replace into UserAccount values('%1','%2',%3)")
                                          .arg(ui->tableAccount->model()->index(row,0).data().toString())
                                          .arg(ui->tableAccount->model()->index(row,1).data().toString())
                                          .arg(ui->tableAccount->model()->index(row,2).data().toInt()));
    listQuery.append(query);
}
void MainWindow::on_actionAccount_triggered()
{
    AddAccountForm * frm = new AddAccountForm();
    frm->show();

}

