#include "querystudentform.h"
#include "ui_querystudentform.h"

QueryStudentForm::QueryStudentForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryStudentForm)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    ui->listResult->setModel(model);
}

QueryStudentForm::~QueryStudentForm()
{
    delete ui;
}

void QueryStudentForm::on_txtName_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        return;
    QStringList list;

    QMapIterator<QString, QString> i(mapStudent);
      while (i.hasNext()) {
          i.next();
          if(i.value().contains(arg1))
          {
              list << i.key() + "-" + i.value();
          }
      }
      model->setStringList(list);
}

void QueryStudentForm::initializeData()
{
    QSqlQuery query;
    query.exec(QString("select id,name from StudentInfo where class=%1").arg(classNumber));
    while(query.next())
        mapStudent.insert(query.value(0).toString(), query.value(1).toString());
}

void QueryStudentForm::on_listResult_doubleClicked(const QModelIndex &index)
{
    emit confirmSingleStudent(index.data().toString().split('-').at(0));
    this->close();
}

QString QueryStudentForm::getClassNumber() const
{
    return classNumber;
}

void QueryStudentForm::setClassNumber(const QString &value)
{
    classNumber = value;
}
