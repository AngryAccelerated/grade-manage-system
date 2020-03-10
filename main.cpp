#include "mainwindow.h"
#include "loginform.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QFont font("微软雅黑");
    QApplication::setFont(font);
    QApplication a(argc, argv);
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./database/grade_db.db");
    if(db.open())
        qDebug() << "database open successfully";
    QTranslator translator;
    translator.load(":/locale/chinese.qm");

    a.installTranslator(&translator);
    LoginForm lf;
    lf.show();
    lf.setApp(&a);
    return a.exec();
}
