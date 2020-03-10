#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <mainwindow.h>
namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();
    void closeEvent(QCloseEvent *event);
    QApplication *getApp() const;
    void setApp(QApplication *value);

public slots:
    void clearAccountAndShow();
private slots:
    void on_btnLogin_clicked();

private:
    Ui::LoginForm *ui;
    MainWindow mainwin;
    void loginAccount(const QString &account, const QString &password);
    QApplication *app;
};

#endif // LOGINFORM_H
