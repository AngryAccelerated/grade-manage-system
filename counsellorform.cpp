#include "counsellorform.h"
#include "ui_counsellorform.h"

CounsellorForm::CounsellorForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CounsellorForm)
{
    ui->setupUi(this);

    connect(ui->acExport,SIGNAL(triggered()),this,SLOT(saveClassGrade()));
    connect(ui->acQuery,SIGNAL(triggered()),this,SLOT(querySingleStudent()));
    connect(ui->acStatistics,SIGNAL(triggered()),this,SLOT(plotTotalClassGrade()));
    connect(ui->acQueryByGrade,SIGNAL(triggered()),this,SLOT(queryStudentByGrade()));
}

CounsellorForm::~CounsellorForm()
{
    delete ui;
}

void CounsellorForm::initializeData()
{
    qDebug() << "Counsellor id" << id;
    QSqlQuery query;
    query.exec(QString("select class_number,counsellor_name from ClassInfo where counsellor_id='%1'").arg(id));
    qDebug() << QString("select class_number,counsellor_name from ClassInfo where counsellor_id='%1'").arg(id);
    while(query.next())
    {
        classNumber = query.value(0).toString();
        ui->lblName->setText(ui->lblName->text().append(query.value(1).toString()));
        ui->lblClass->setText(ui->lblClass->text().append(classNumber));
        ui->lblAccountID->setText(ui->lblAccountID->text() + id);

    }

    queryClassGradeInfo();
    calculateAverageGrade();
}

void CounsellorForm::queryClassGradeInfo()
{
    QStandardItemModel* model = new QStandardItemModel();
    QStringList labels = QString(tr("Student ID,Name,Gender,Faculty,Major,Total Grade,Level,Action")).split(",");
    model->setHorizontalHeaderLabels(labels);

    ui->tableView->setModel(model);
    int i = 0, great_student_count = 0;
    QSqlQuery query;
    query.exec(QString("select * from StudentInfo where class=%1").arg(classNumber));
    qDebug() << QString("select * from StudentInfo where class=%1").arg(classNumber);
    while(query.next())
    {
        QStandardItem* item = nullptr;
        for(int j = 0;j < 5;j++)
        {
            if(j == 2)
            {
                if(query.value(j).toInt() == 1)
                    item = new QStandardItem(tr("male"));
                else
                    item = new QStandardItem(tr("female"));

                model->setItem(i,j,item);
            }
            else
            {
                item = new QStandardItem(query.value(j).toString());
                model->setItem(i,j,item);
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }

        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        double total_grade = calculateTheTotalGrade(query.value(0).toString());
        item = new QStandardItem(QString::number(total_grade));
        model->setItem(i,5,item);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        QString level = evaluateStudentGradeLevel(total_grade);
        item = new QStandardItem(level);
        model->setItem(i,6,item);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        if(level == tr("EXCELLENT") || level == tr("GOOD"))
            great_student_count++;
        QPushButton * btnDetail = new QPushButton(ui->tableView);
        btnDetail->setText(tr("Detail"));
        btnDetail->setProperty("sid",query.value(0));
        ui->tableView->setIndexWidget(model->index(i,7), btnDetail);
        connect(btnDetail,SIGNAL(clicked()),this, SLOT(displaySingleStudentInfo()));
        i++;
    }

    model->sort(5,Qt::DescendingOrder);
    ui->lblStudentCount->setText(ui->lblStudentCount->text() + QString::number(i));
    ui->lblGreatStudent->setText(ui->lblGreatStudent->text() + QString::number(great_student_count));
}

double CounsellorForm::calculateTheTotalGrade(const QString& id)
{
    double total_grade = 0;
    int total_credit = 0;
    QSqlQuery query;
    query.exec(QString("select course_id,course_grade from StudentCourseInfo_%1").arg(id));
    while(query.next())
    {
        QSqlQuery query2;
        query2.exec(QString("select course_credit from CourseInfo where course_id='%1'").arg(query.value(0).toString()));
        while(query2.next())
        {
            int credit = query2.value(0).toInt();
            double course_grade = query.value(1).toInt();
            course_grade = evaluateCourseGrade(course_grade);
            total_credit += credit;
            total_grade += credit * course_grade;
        }
    }
    qDebug() << "total credit: " <<total_grade / (total_credit * 1.0);
    return total_grade / (total_credit * 1.0);
}

void CounsellorForm::displaySingleStudentInfo()
{
    QString sid = static_cast<QPushButton*>(sender())->property("sid").toString();
    StudentForm * frmStudent = new StudentForm();
    frmStudent->setFromWhere("Counsellor");
    frmStudent->setId(sid);
    frmStudent->initializeData();
    frmStudent->show();
}
void CounsellorForm::closeEvent(QCloseEvent *event)
{
    int r = QMessageBox::question(this,tr("Last Hint"),tr("Are you sure you want to quit?"),QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (r==QMessageBox::Yes){
        event->accept();
        emit logout();
    }
    else
        event->ignore();
}
void CounsellorForm::plotTotalClassGrade()
{
    QChart *chart = new QChart();
    chart->setTitle(tr("Class Grade Pie Chart"));
    QChartView *chartView = new QChartView(chart);
    chartView->setWindowTitle(tr("Class Grade Chart"));

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chartView->setRenderHint(QPainter::Antialiasing);


    QPieSeries * series = new QPieSeries(chart);
    chart->addSeries(series);

    QPieSlice * psExcellent = new QPieSlice();
    psExcellent->setLabel(tr("EXCELLENT"));
    psExcellent->setValue(0);
    QPieSlice * psGood = new QPieSlice();
    psGood->setLabel(tr("GOOD"));
    psGood->setValue(0);
    QPieSlice * psSatisfactory = new QPieSlice();
    psSatisfactory->setLabel(tr("SATISFATORY"));
    psSatisfactory->setValue(0);
    QPieSlice * psPass = new QPieSlice();
    psPass->setLabel(tr("PASS"));
    psPass->setValue(0);

    connect(psExcellent,&QPieSlice::hovered,this,&CounsellorForm::explodeHoveredSlice);
    connect(psGood,&QPieSlice::hovered,this,&CounsellorForm::explodeHoveredSlice);
    connect(psSatisfactory,&QPieSlice::hovered,this,&CounsellorForm::explodeHoveredSlice);
    connect(psPass,&QPieSlice::hovered,this,&CounsellorForm::explodeHoveredSlice);

    series->append(psExcellent);
    series->append(psGood);
    series->append(psSatisfactory);
    series->append(psPass);
    for (int i = 0;i < ui->tableView->model()->rowCount();i++) {
        QString level = ui->tableView->model()->index(i,6).data().toString();
        if(level == tr("EXCELLENT"))
        {
            psExcellent->setValue(psExcellent->value() + 1);
        }
        else if(level == tr("GOOD"))
        {
            psGood->setValue(psGood->value() + 1);
        }
        else if(level == tr("SATISFATORY"))
        {
            psSatisfactory->setValue(psSatisfactory->value() + 1);
        }
        else if(level == tr("PASS"))
        {
            psPass->setValue(psPass->value() + 1);
        }
    }
    psExcellent->setLabel(psExcellent->label() + " : " + QString::number(psExcellent->percentage() * 100) + "%");
    psGood->setLabel(psGood->label() + " : " + QString::number(psGood->percentage() * 100) + "%");
    psSatisfactory->setLabel(psSatisfactory->label() + " : " + QString::number(psSatisfactory->percentage() * 100) + "%");
    psPass->setLabel(psPass->label() + " : " + QString::number(psPass->percentage() * 100) + "%");
    chartView->setMinimumSize(640, 480);
    chartView->show();
}

void CounsellorForm::calculateAverageGrade()
{
    double total_grade = 0;
    for (int i = 0;i < ui->tableView->model()->rowCount();i++) {
        total_grade += ui->tableView->model()->index(i,5).data().toDouble();
    }
    ui->lblAverage->setText(ui->lblAverage->text() +
                            QString::number(total_grade / (ui->tableView->model()->rowCount() * 1.0)));
}

void CounsellorForm::saveClassGrade()
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
        for(int i = 0;i < ui->tableView->model()->columnCount()-1;i++)
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

void CounsellorForm::querySingleStudent()
{
    QueryStudentForm * frmQuery = new QueryStudentForm();
    frmQuery->setClassNumber(classNumber);
    frmQuery->initializeData();
    connect(frmQuery,&QueryStudentForm::confirmSingleStudent,this,&CounsellorForm::highlightSingleStudent);
    frmQuery->show();
}

void CounsellorForm::queryStudentByGrade()
{
    QueryStudentGradeForm * frmQuery = new QueryStudentGradeForm();
    connect(frmQuery,&QueryStudentGradeForm::confirmQueryRange, this,&CounsellorForm::highlightRangeStudent);
    frmQuery->show();
}

QString CounsellorForm::evaluateStudentGradeLevel(const double &total_grade)
{
    if(total_grade == 4.0)
    {
        return tr("EXCELLENT");
    }
    else if(total_grade >= 3.5)
    {
        return tr("GOOD");
    }
    else if(total_grade >= 2.5)
    {
        return tr("SATISFATORY");
    }
    else if(total_grade >= 1.5)
    {
        return tr("PASS");
    }
}


double CounsellorForm::evaluateCourseGrade(const double &course_grade)
{
    if(course_grade >= 85 && course_grade <= 100)
    {
        return 4.0;
    }
    else if(course_grade >= 60 && course_grade <= 84)
    {
        return 3.9 - 0.1 * (84 - course_grade);
    }
    return 0;
}

void CounsellorForm::explodeHoveredSlice(bool show)
{
    QPieSlice * slice = static_cast<QPieSlice*>(sender());
    slice->setExploded(show);
    slice->setLabelVisible(show);
}

void CounsellorForm::highlightSingleStudent(const QString &id)
{
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    for(int i = 0;i < ui->tableView->model()->rowCount();i++)
    {
        if(ui->tableView->model()->index(i,0).data().toString() == id)
            ui->tableView->selectRow(i);
    }
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void CounsellorForm::highlightRangeStudent(const qreal &maximum, const qreal &minimum)
{
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    for(int i = 0;i < ui->tableView->model()->rowCount();i++)
    {
        if(ui->tableView->model()->index(i,5).data().toDouble() >= minimum && ui->tableView->model()->index(i,5).data().toDouble() <= maximum)
            ui->tableView->selectRow(i);
    }
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

QString CounsellorForm::getId() const
{
    return id;
}

void CounsellorForm::setId(const QString &value)
{
    id = value;
}

void CounsellorForm::on_acLogout_triggered()
{
    this->hide();
    emit logout();
}

void CounsellorForm::on_actionSort_By_ID_triggered()
{
}
