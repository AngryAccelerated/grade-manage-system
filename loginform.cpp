#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    ui->btnLogin->setShortcut(Qt::Key_Enter);
    ui->btnLogin->setShortcut(QKeySequence::InsertParagraphSeparator);
    ui->btnLogin->setShortcut(Qt::Key_Return);
    connect(&mainwin,&MainWindow::displayLoginForm,this,&LoginForm::clearAccountAndShow);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::clearAccountAndShow()
{
    ui->txtAccount->setText("");
    ui->txtPassword->setText("");
    ui->btnLogin->setEnabled(true);
    this->show();
}
void LoginForm::closeEvent(QCloseEvent *event)
{
    this->close();

}
void LoginForm::loginAccount(const QString &account, const QString &password)
{

    ui->btnLogin->setEnabled(false);
    if(mainwin.loginAccount(account,password))
    {
        qDebug() << "login successfully, id:" << account;
        this->hide();
        mainwin.displayWindow();
    }
    else{
        qDebug() << "fail to login";
        QMessageBox msgBox;
        msgBox.setText(tr("Fail to login, please check your account and password."));
        msgBox.exec();
        ui->btnLogin->setEnabled(true);
    }
}

QApplication *LoginForm::getApp() const
{
    return app;
}

void LoginForm::setApp(QApplication *value)
{
    app = value;
}

void LoginForm::on_btnLogin_clicked()
{
    loginAccount(ui->txtAccount->text(), ui->txtPassword->text());
}
