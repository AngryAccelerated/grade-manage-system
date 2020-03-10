#include "teacherform.h"
#include "ui_teacherform.h"

TeacherForm::TeacherForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeacherForm)
{
    ui->setupUi(this);
}

TeacherForm::~TeacherForm()
{
    delete ui;
}

QString TeacherForm::getId() const
{
    return id;
}

void TeacherForm::setId(const QString &value)
{
    id = value;
}

void TeacherForm::initializeData()
{
    QSqlQuery query;
    query.exec(QString("select name from TeacherInfo where id='%1'").arg(id));
    while(query.next())
    {
        ui->txtName->setText(query.value(0).toString());
    }
    ui->txtAccountID->setText(id);
    query.exec(QString("select * from CourseInfo where teacher_id = '%1'").arg(id));
    while(query.next())
    {
        query.value(0);//course_id
        query.value(1);//course_name
        query.value(3);//course_period
        query.value(4);//course_credit
        mapCourseCredit[query.value(0).toString()] = query.value(4).toInt();
        mapCourseID[query.value(1).toString()] = query.value(0).toString();
        ui->cbbCourse->addItem(query.value(1).toString());
        QSqlQuery query2;
        int term = 0;
        //term validity judgement
        for(int i = 1;i < 7;i++)
        {
            query2.exec(QString("select * from Course_%1_Teacher_%2_Term_%3").arg(query.value(0).toString()).arg(id).arg(i));
            while(query2.next())
            {
                if(query2.isValid())
                {
                    term++;
                    break;
                }
            }
        }
        if(term > 0)
        {

            for(int i = 1;i <= term;i++)
                ui->cbbTerm->addItem(QString::number(i));
        }
        mapCourse[query.value(0).toString()] = term;
    }
    loadGradeTable();
}

void TeacherForm::on_acLogout_triggered()
{
    this->hide();

    emit logout();
}

void TeacherForm::calculateAverage()
{
    //calculate the total count
    ui->txtCount->setText(QString::number(ui->tableView->model()->rowCount()));
    //calculate the average grade
    double result = 0;
    for(int i = 0;i < ui->tableView->model()->rowCount();i++)
    {
        result += ui->tableView->model()->index(i,2).data().toDouble();
    }
    result /= ui->tableView->model()->rowCount();
    ui->txtAverage->setText(QString::number(result));

}
void TeacherForm::closeEvent(QCloseEvent *event)
{
    int r = QMessageBox::question(this,tr("Last Hint"),tr("Are you sure you want to quit?"),QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (r==QMessageBox::Yes)
    {
        event->accept();
        emit logout();
    }
    else
        event->ignore();
}
void TeacherForm::importXlsxFile()
{

}

void TeacherForm::on_cbbCourse_currentIndexChanged(const QString &arg1)
{
    QString courseID = mapCourseID[arg1];
    int term = mapCourse[courseID];
    ui->cbbTerm->clear();
    for(int i = 1;i <= term;i++)
    {
        ui->cbbTerm->addItem(QString::number(i));
    }
}

void TeacherForm::loadGradeTable()
{
    //ui->tableView->model()->removeRows(0,ui->tableView->model()->rowCount());

    ui->tableView->reset();
    QStandardItemModel * model = new QStandardItemModel();

    QStringList labels = QString(tr("Student ID,Name,Grade,Faculty")).split(",");
    model->setHorizontalHeaderLabels(labels);
    QString current_course = mapCourseID[ui->cbbCourse->currentText()];
    int current_term = ui->cbbTerm->currentIndex() + 1;
    QSqlQuery query;
    query.exec(QString("select * from Course_%1_Teacher_%2_Term_%3").arg(current_course).arg(id).arg(current_term));
    int i = 0;
    while(query.next())
    {
        query.value(0);//id;
        query.value(1);//grade;
        QSqlQuery query2;
        query2.exec(QString("select name,faculty from StudentInfo where id = %1").arg(query.value(0).toString()));
        while(query2.next())
        {
            QStandardItem * item = new QStandardItem(query.value(0).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            model->setItem(i,0,item);

            item = new QStandardItem(query2.value(0).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            model->setItem(i,1,item);

            item = new QStandardItem(query.value(1).toString());
            model->setItem(i,2,item);

            item = new QStandardItem(query2.value(1).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            model->setItem(i,3,item);
            i++;
        }
    }
    ui->tableView->setModel(model);
    calculateAverage();
    connect(model,&QStandardItemModel::dataChanged,this,&TeacherForm::tableDataChanged);
}


void TeacherForm::on_cbbTerm_currentIndexChanged(int index)
{
    loadGradeTable();
}


void TeacherForm::on_acExport_triggered()
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
        for(int i = 0;i < ui->tableView->model()->columnCount();i++)
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

void TeacherForm::on_acImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Excel File"), "",
                                                    tr("Excel Format(*.xlsx)"));

    if(fileName.isEmpty())
        return;
    else
    {
        if(fileName.right(5) != ".xlsx")
            fileName += ".xlsx";

        QXlsx::Document xlsxR(fileName);
        qDebug() << fileName;
        QMessageBox msgBox;
        int rowIndex = 0;
        qDebug() << xlsxR.read("A1");
        if(!xlsxR.read("A1").isNull())
        {
            ui->tableView->reset();
            QStandardItemModel * model = new QStandardItemModel();

            QStringList labels = QString(tr("Student ID,Name,Grade,Faculty")).split(",");
            model->setHorizontalHeaderLabels(labels);
            ui->tableView->setModel(model);
            while(1)
            {
                if(!xlsxR.read(rowIndex + 2,1).isNull())
                {
                    qDebug() << "i: " << rowIndex << "-" << xlsxR.read(rowIndex + 1,1).toString();
                    model->setItem(rowIndex,0,new QStandardItem(xlsxR.read(rowIndex + 2,1).toString()));
                    model->setItem(rowIndex,1,new QStandardItem(xlsxR.read(rowIndex + 2,2).toString()));
                    model->setItem(rowIndex,2,new QStandardItem(xlsxR.read(rowIndex + 2,3).toString()));
                    model->setItem(rowIndex,3,new QStandardItem(xlsxR.read(rowIndex + 2,4).toString()));
                    rowIndex++;
                }
                else{
                    qDebug() << "i: "<< rowIndex << "-END";
                    break;
                }
            }
            msgBox.setText(tr("Import successfully."));
            msgBox.exec();
            connect(model,&QStandardItemModel::dataChanged,this,&TeacherForm::tableDataChanged);
        }
        else
        {
            msgBox.setText(tr("Fail to Import."));
            msgBox.exec();
        }
    }
}

void TeacherForm::on_acSave_triggered()
{
    QString courseID = mapCourseID[ui->cbbCourse->currentText()];
    int term = ui->cbbTerm->currentIndex() + 1;
    QSqlQuery query;
    for(int i = 0;i < ui->tableView->model()->rowCount();i++)
    {
        query.exec(QString("update Course_%1_Teacher_%2_Term_%3 set grade=%4").arg(courseID).arg(id).arg(term)
                   .arg(ui->tableView->model()->index(i,2).data().toInt()));
    }
    foreach (QSqlQuery* query, listQuery) {
        query->exec();
        delete query;
    }
}

void TeacherForm::on_acStatistics_triggered()
{
    QString courseID = ui->cbbCourse->currentText();
    int term = ui->cbbCourse->currentIndex() + 1;
    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);
    chartView->setWindowTitle(tr("Pie Chart"));
    chart->setTitle(tr("Course:%1 Term:%2 Grade Pie Chart").arg(courseID).arg(term));

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
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
    QPieSlice * psUnPass = new QPieSlice();
    psUnPass->setLabel(tr("UNPASS"));
    psUnPass->setValue(0);

    connect(psExcellent,&QPieSlice::hovered,this,&TeacherForm::explodeHoveredSlice);
    connect(psGood,&QPieSlice::hovered,this,&TeacherForm::explodeHoveredSlice);
    connect(psSatisfactory,&QPieSlice::hovered,this,&TeacherForm::explodeHoveredSlice);
    connect(psPass,&QPieSlice::hovered,this,&TeacherForm::explodeHoveredSlice);
    connect(psUnPass,&QPieSlice::hovered,this,&TeacherForm::explodeHoveredSlice);

    series->append(psExcellent);
    series->append(psGood);
    series->append(psSatisfactory);
    series->append(psPass);
    series->append(psUnPass);
    for (int i = 0;i < ui->tableView->model()->rowCount();i++) {
        QString level = transferGradeToLevel(ui->tableView->model()->index(i,2).data().toInt());
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
        else if(level == tr("UNPASS"))
        {
            psUnPass->setValue(psUnPass->value() + 1);
        }
    }
    psExcellent->setLabel(psExcellent->label() + " : " + QString::number(psExcellent->percentage() * 100) + "%");
    psGood->setLabel(psGood->label() + " : " + QString::number(psGood->percentage() * 100) + "%");
    psSatisfactory->setLabel(psSatisfactory->label() + " : " + QString::number(psSatisfactory->percentage() * 100) + "%");
    psPass->setLabel(psPass->label() + " : " + QString::number(psPass->percentage() * 100) + "%");
    psUnPass->setLabel(psUnPass->label() + " : " + QString::number(psUnPass->percentage() * 100) + "%");
    chartView->setMinimumSize(640, 480);
    chartView->show();
}

void TeacherForm::on_actionDelete_triggered()
{
    QString courseID = mapCourseID[ui->cbbCourse->currentText()];
    int term = ui->cbbTerm->currentIndex() + 1;
    QString currentStudentID = ui->tableView->model()->data(ui->tableView->currentIndex()).toString();
    QSqlQuery * query = new QSqlQuery(QString("delete from Course_%1_Teacher_%2_Term_%3 where id='%4'").arg(courseID).arg(id).arg(term).arg(currentStudentID));
    listQuery.append(query);
    qDebug() << QString("delete from Course_%1_Teacher_%2_Term_%3 where id='%4'").arg(courseID).arg(id).arg(term).arg(currentStudentID);
    ui->tableView->model()->removeRow(ui->tableView->currentIndex().row());
}

void TeacherForm::explodeHoveredSlice(bool show)
{
    QPieSlice * slice = static_cast<QPieSlice*>(sender());
    slice->setExploded(show);
    slice->setLabelVisible(show);
}

const QString TeacherForm::transferGradeToLevel(const int &grade)
{
    if(grade >= 85)
    {
        return tr("EXCELLENT");
    }
    else if(grade >= 80)
    {
        return tr("GOOD");
    }
    else if(grade >= 70)
    {
        return tr("SATISFATORY");
    }
    else if(grade >= 60)
    {
        return tr("PASS");
    }
    else{
        return tr("UNPASS");
    }
}

void TeacherForm::tableDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    QString studentID = ui->tableView->model()->index(topLeft.row(),0).data().toString();

    QString courseID = mapCourseID[ui->cbbCourse->currentText()];
    int term = ui->cbbTerm->currentIndex() + 1;
    int grade = ui->tableView->model()->data(topLeft).toInt();
    QSqlQuery * query = new QSqlQuery(QString("update Course_%1_Teacher_%2_Term_%3 set grade=%4 where id='%5'")
                                      .arg(courseID).arg(id).arg(term).arg(grade).arg(studentID));
    listQuery.append(query);
    qDebug() << QString("update Course_%1_Teacher_%2_Term_%3 set grade=%4 where id='%5'")
                .arg(courseID).arg(id).arg(term).arg(grade).arg(studentID);
}

void TeacherForm::on_actionSave_All_triggered()
{
    for(int i = 1;i <= ui->cbbTerm->count();i++)
    {
        int term = i;
        QString courseID = mapCourseID[ui->cbbCourse->itemText(i - 1)];
        for(int j = 0;j < ui->tableView->model()->rowCount();j++)
        {
            int grade = ui->tableView->model()->index(j,2).data().toInt();
            qDebug() << grade;
            QString studentID = ui->tableView->model()->index(j,0).data().toString();
            QSqlQuery * query = new QSqlQuery(QString("update Course_%1_Teacher_%2_Term_%3 set grade=%4 where id='%5'")
                                              .arg(courseID).arg(id).arg(term).arg(grade).arg(studentID));
            listQuery.append(query);
        }
    }
    QMessageBox msgBox;
    msgBox.setText(tr("Done"));
    msgBox.exec();
}
