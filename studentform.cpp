#include "studentform.h"
#include "ui_studentform.h"

StudentForm::StudentForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentForm)
{
    ui->setupUi(this);

    connect(ui->acLineSeries,SIGNAL(triggered()),this,SLOT(plotGradeLineGraph()));
    connect(ui->acBarSeries,SIGNAL(triggered()),this,SLOT(plotGradeGraph()));
    connect(ui->acExport,SIGNAL(triggered()),this,SLOT(saveGradeTable()));
    connect(ui->acQuerySingleSubject,SIGNAL(triggered()),this,SLOT(blurQuerySingleSubject()));


    //ui->tableView->verticalHeader()->hide();
}

StudentForm::~StudentForm()
{
    delete ui;
}

void StudentForm::queryStudentTotalGrade()
{
    QStandardItemModel* model = new QStandardItemModel();
    QStringList labels = QString(tr("CourseID,Course Name,Credit,Course Period,Grade,Term")).split(",");
    model->setHorizontalHeaderLabels(labels);

    QSqlQuery query;
    query.exec(QString("select course_id,term,course_grade from StudentCourseInfo_%1").arg(id));
    int i = 0;
    while(query.next())
    {
        QSqlQuery query2;
        //the genre of course_id is TEXT, so the '' is needed
        query2.exec(QString("select course_name,course_period,course_credit from CourseInfo where course_id='%1'").arg(query.value(0).toString()));
        while(query2.next())
        {
            QStandardItem* item = nullptr;

            item = new QStandardItem(query.value(0).toString());
            model->setItem(i,0,item);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            item = new QStandardItem(query2.value(0).toString());
            model->setItem(i,1,item);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            item = new QStandardItem(query2.value(2).toString());
            model->setItem(i,2,item);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            item = new QStandardItem(query2.value(1).toString());
            model->setItem(i,3,item);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            item = new QStandardItem(query.value(2).toString());
            model->setItem(i,4,item);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            item = new QStandardItem(query.value(1).toString());
            model->setItem(i,5,item);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            qDebug() << query.value(0).toString() << "-" << query2.value(0).toString() << "-" << query2.value(2).toString()
                     << "-" << query2.value(1).toString() << "-" << query.value(2).toString() << query.value(1).toString();
        }
        i++;
    }
    model->sort(5);
    ui->tableView->setModel(model);
    ui->tableView->show();
}

void StudentForm::queryStudentBasicInfo()
{
    QSqlQuery query;
    query.exec(QString("select name,gender,faculty,major,class from StudentInfo where id = '%1'").arg(id));
    while(query.next())
    {
        ui->lblStudentID->setText(ui->lblStudentID->text().append(id));
        ui->lblName->setText(ui->lblName->text().append(query.value(0).toString()));
        query.value(1).toInt() == 1 ? ui->lblGender->setText(ui->lblGender->text().append("男"))
                                    : ui->lblGender->setText(ui->lblGender->text().append("女"));
        ui->lblFaculty->setText(ui->lblFaculty->text().append(query.value(2).toString()));
        ui->lblMajor->setText(ui->lblMajor->text().append(query.value(3).toString()));
        ui->lblClass->setText(ui->lblClass->text().append(query.value(4).toString()));
    }
}

QString StudentForm::getId() const
{
    return id;
}

void StudentForm::setId(const QString &value)
{
    id = value;
}

void StudentForm::initializeData()
{
    qDebug() << "student id:" << id;
    queryStudentBasicInfo();
    queryStudentTotalGrade();

}

void StudentForm::plotGradeGraph()
{
    qDebug() << "plot the bar graph";

    QChart *chart = new QChart;
    QChartView *chartView = new QChartView(chart);
    chartView->setWindowTitle(tr("Bar Graph"));
    QMap<QString, QBarSet*> mapBarset;

    QStringList categories;
    categories << tr("FIRST") << tr("SECOND") << tr("THIRD") << tr("FOURTH") << tr("FIFTH") << tr("SIXTH");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    QValueAxis *axisY = new QValueAxis();

    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY->setRange(0,100);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->setTitle(tr("Student Graph"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setMinimumSize(640, 480);
    for (int i = 0;i < ui->tableView->model()->rowCount();i++) {
        QString str = ui->tableView->model()->index(i,1).data().toString();
        int grade = ui->tableView->model()->index(i,4).data().toInt();
        if(mapBarset.contains(str))
        {
            mapBarset[str]->append(grade);
        }
        else
        {
            QBarSet * barset = new QBarSet(str);
            QBarSeries * series = new QBarSeries();

            *barset << grade;
            series->append(barset);
            chart->addSeries(series);
            series->attachAxis(axisX);
            series->attachAxis(axisY);
            mapBarset.insert(str,barset);
        }
    }
    chartView->show();
}

void StudentForm::plotGradeLineGraph()
{
    qDebug() << "plot the line graph";

     QChart *chart = new QChart;
     QChartView *chartView = new QChartView(chart);
     chartView->setWindowTitle(tr("Line Graph"));
     QStringList lstCourseName;
     QMap<QString, QLineSeries*> mapData;
     for (int i = 0;i < ui->tableView->model()->rowCount();i++) {
         QString str = ui->tableView->model()->index(i,1).data().toString();
         int grade = ui->tableView->model()->index(i,4).data().toInt();
         int term = ui->tableView->model()->index(i,5).data().toInt();
         qDebug() << str << "    " << grade << " " << term;
         if(!mapData.contains(str)){
             QLineSeries * series = new QLineSeries(chart);
             series->setName(str);
             series->append(term,grade);
             mapData.insert(str,series);
             chart->addSeries(series);
         }
         else{
             mapData.value(str)->append(term,grade);
         }

     }

     chart->setAnimationOptions(QChart::AllAnimations);
     chart->createDefaultAxes();
     chart->axisX()->setRange(1,6);
     chart->axisY()->setRange(0,100);

     chartView->setRenderHint(QPainter::Antialiasing);
     chartView->setMinimumSize(640, 480);
     chart->setTitle(tr("Grade Graph"));

     chartView->show();

}

void StudentForm::saveGradeTable()
{
    QXlsx::Document xlsx;
    QString fileName = QFileDialog::getSaveFileName(this,
           tr("Save Excel File"), "",
           tr("Excel Format(*.xlsx)"));

    if(fileName.isEmpty())
        return;
    else
    {
        if(fileName.right(5) != ".xlsx")
            fileName += ".xlsx";
        for(int i = 0;i < ui->tableView->model()->columnCount()+1;i++)
        {

            xlsx.write(1,i + 1,ui->tableView->horizontalHeader()->model()->headerData(i,Qt::Horizontal));
            for(int j = 0;j < ui->tableView->model()->rowCount();j++)
                xlsx.write(j + 2,i + 1,ui->tableView->model()->index(j,i).data());

        }
        QMessageBox msgBox;
        if(xlsx.saveAs(fileName))
            msgBox.setText(tr("Export successfully."));
        else
            msgBox.setText(tr("Fail to export."));
        msgBox.exec();
    }
}
void StudentForm::closeEvent(QCloseEvent *event)
{
    if(fromWhere == "Login")
    {
        int r = QMessageBox::question(this,tr("Last Hint"),tr("Are you sure you want to quit?"),QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
        if (r==QMessageBox::Yes){
            event->accept();
            emit logout();
        }
        else
            event->ignore();
    }
    else if(fromWhere == "Counsellor")
    {
        this->close();
    }
}
void StudentForm::blurQuerySingleSubject()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Query Grade"),
                                         tr("Please input the name of course"), QLineEdit::Normal,
                                         "", &ok);
    qDebug() << "course name" << text;

    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    if (ok && !text.isEmpty()){
        ui->tableView->clearSelection();
        for (int i = 0;i < ui->tableView->model()->rowCount();i++)
            if(ui->tableView->model()->index(i,1).data().toString().contains(text))
                ui->tableView->selectRow(i);
    }
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void StudentForm::on_acExit_triggered()
{
    this->hide();
    emit logout();
}

QString StudentForm::getFromWhere() const
{
    return fromWhere;
}

void StudentForm::setFromWhere(const QString &value)
{
    fromWhere = value;
}
