#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // database 連結
    QSqlDatabase db(QSqlDatabase::addDatabase("QMYSQL"));
    db.setHostName("127.0.0.1");
    db.setDatabaseName("DBMS");
    db.setUserName("ariel");
    db.setPassword("aaa");
    if (!db.open()) /*測試database是否連接成功*/
    {
       qDebug() << db.lastError() << endl;
    }
    else
    {
       qDebug() << "Ok" << endl;
    }
}

void MainWindow::getsql() {
    QString q=this->ui->textEdit->toPlainText();
    QSqlQuery query;

    query.exec(q);
    QSqlRecord rec = query.record();
    QStringList header;

    for (int var = 0; var < rec.count(); ++var) {
        QString colName = rec.fieldName(var);
        header.append(colName);
    }

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);
    if(!query.isActive()) {
        this->ui->label->setText("指令失敗");
        return;
    }
    this->ui->label->setText("指令成功");
    // set table
    ui->tableWidget->setColumnCount(rec.count());
    ui->tableWidget->setRowCount(query.size());
    ui->tableWidget->setHorizontalHeaderLabels(header);

    int row=0;
    while(query.next())
    {
        int column=0;
        for(QString v:header) {
            QString value = query.value(v).toString();
            ui->tableWidget->setItem(row, column, new QTableWidgetItem(value));
            column++;
        }
        row++;
    }
      //select * from 買家
}

void MainWindow::searchByProductName() {
    QString name=this->ui->textEdit_2->toPlainText();
    QString q = "select 商品.ID,品名,價格,商店名稱 from 商品, 賣家 where 品名='" + name +"' and 商品.賣家ID=賣家.ID;";
    QSqlQuery query;

    query.exec(q);
    QSqlRecord rec = query.record();
    QStringList header;

    for (int var = 0; var < rec.count(); ++var) {
        QString colName = rec.fieldName(var);
        header.append(colName);
    }
    // set table
    ui->tableWidget_2->clear();
    if(!query.size()) {
        ui->label_2->setText("查無商品");
    }else {
        ui->label_2->setText("查詢成功");
    }
    ui->tableWidget_2->setColumnCount(rec.count());
    ui->tableWidget_2->setRowCount(query.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(header);

    int row=0;
    while(query.next())
    {
        int column=0;
        for(QString v:header) {
            QString value = query.value(v).toString();
            ui->tableWidget_2->setItem(row, column, new QTableWidgetItem(value));
            column++;
        }
        row++;
    }
}

void MainWindow::addShoppingList(){
    QString q="insert into 購物清單 (買家ID, 商品ID, 數量, 是否加入訂單, 訂單ID) values (" + ui->textEdit_3->toPlainText() +","+ui->textEdit_4->toPlainText()+","+ui->textEdit_5->toPlainText()+", 0, NULL);";
    QSqlQuery query;
    query.exec(q);
    QMessageBox msgBox;
    if(!query.isActive()) {
        msgBox.setText("新增失敗");
        msgBox.exec();
    } else {
        msgBox.setText("新增成功");
        msgBox.exec();
    }
}

void MainWindow::searchShoppingList(){
    QString q="select 購物清單.ID, 品名, 商店名稱, 數量, 是否加入訂單  from 購物清單, 商品, 賣家 where 購物清單.商品ID=商品.ID and 商品.賣家ID=賣家.ID and 買家ID="+ui->textEdit_6->toPlainText() +";";
    QSqlQuery query;

    query.exec(q);
    QSqlRecord rec = query.record();
    QStringList header;

    for (int var = 0; var < rec.count(); ++var) {
        QString colName = rec.fieldName(var);
        header.append(colName);
    }
    header.append("加到訂單");
    header.append("是否刪除");

    // set table
    ui->tableWidget_3->clear();
    QMessageBox msgBox;
    if(!query.size()) {
        msgBox.setText("查無資料");
        msgBox.exec();
    }else {
        msgBox.setText("查詢成功");
        msgBox.exec();
    }
    ui->tableWidget_3->setColumnCount(rec.count()+2);
    ui->tableWidget_3->setRowCount(query.size());
    ui->tableWidget_3->setHorizontalHeaderLabels(header);

    int row=0;
    while(query.next())
    {
        int column=0;
        for(int i=0; i<rec.count(); i++) {
            QString value = query.value(header[i]).toString();
            ui->tableWidget_3->setItem(row, column, new QTableWidgetItem(value));
            column++;
        }
        QTableWidgetItem *item = new QTableWidgetItem();
        QTableWidgetItem *item2 = new QTableWidgetItem();
        if(!query.value(header[4]).toInt()){
            item->setCheckState(Qt::Unchecked);
            item2->setCheckState(Qt::Unchecked);
            ui->tableWidget_3->setItem(row, column, item);
            ui->tableWidget_3->setItem(row, column+1, item2);
        }
        row++;
    }
}

void MainWindow::addOrder(){
    QString list="(";
    for(int i=0;i<ui->tableWidget_3->rowCount();i++) {
        if(ui->tableWidget_3->item(i, 5) && ui->tableWidget_3->item(i, 5)->checkState()) {
            list.append(ui->tableWidget_3->item(i, 0)->text()+",");
        }
    }
    list[list.size()-1]=')';
    QString q="insert into 訂單(是否評價, 評分) values (0, NULL);";
    QSqlQuery query;
    query.exec(q);
    QMessageBox msgBox;
    if(!query.isActive()) {
        msgBox.setText("新增訂單失敗");
        msgBox.exec();
        return;
    }
    q="SELECT MAX(id) FROM 訂單;";
    query.exec(q);
    if(!query.isActive()) {
        msgBox.setText("新增訂單失敗");
        msgBox.exec();
        return;
    }
    while(query.next()){
        if(ui->radioButton_2->isChecked()) {
            q="update 購物清單 set 是否加入訂單=1, 訂單ID="+query.value(0).toString()+" where 是否加入訂單=0 and ID not in "+list+";";
        } else {
            q="update 購物清單 set 是否加入訂單=1, 訂單ID="+query.value(0).toString()+" where ID in "+list+";";
        }
    }
    query.exec(q);
    if(!query.isActive()) {
        msgBox.setText("加入失敗");
        msgBox.exec();
    }else {
        msgBox.setText("加入成功");
        msgBox.exec();
    }

}

void MainWindow::deleteShoppingList(){
    QString list="(";
    for(int i=0;i<ui->tableWidget_3->rowCount();i++) {
        if(ui->tableWidget_3->item(i, 6) && ui->tableWidget_3->item(i, 6)->checkState()) {
            list.append(ui->tableWidget_3->item(i, 0)->text()+",");
        }
    }
    list[list.size()-1]=')';

    QString q="delete from 購物清單 where ID="+list+";";
    QSqlQuery query;
    query.exec(q);
    QMessageBox msgBox;
    if(!query.isActive()) {
        msgBox.setText("刪除失敗");
        msgBox.exec();
    }else {
        msgBox.setText("刪除成功");
        msgBox.exec();
    }
}

void MainWindow::searchOrder(){
    ui->tableWidget_4->clear();
    ui->tableWidget_4->setColumnCount(0);
    ui->tableWidget_4->setRowCount(0);

    QString q;
    if(ui->radioButton->isChecked()) {
        q="select * from 訂單 where not exists (select * from 購物清單 where 買家ID="+ui->textEdit_7->toPlainText() +" and 訂單.ID=訂單ID);";
    } else {
        q="select * from 訂單 where exists (select * from 購物清單 where 買家ID="+ui->textEdit_7->toPlainText() +" and 訂單.ID=訂單ID);";
    }
    QSqlQuery query;
    query.exec(q);
    QMessageBox msgBox;
    if(!query.size() || !query.isActive()) {
        msgBox.setText("查無資料");
        msgBox.exec();
        return;
    }
    QSqlRecord rec = query.record();
    QStringList header;

    for (int var = 0; var < rec.count(); ++var) {
        QString colName = rec.fieldName(var);
        header.append(colName);
    }
    // set table

    ui->tableWidget_4->setColumnCount(rec.count());
    ui->tableWidget_4->setRowCount(query.size());
    ui->tableWidget_4->setHorizontalHeaderLabels(header);

    int row=0;
    while(query.next())
    {
        int column=0;
        for(QString v:header) {
            QString value = query.value(v).toString();
            ui->tableWidget_4->setItem(row, column, new QTableWidgetItem(value));
            column++;
        }
        row++;
    }
}

void MainWindow::scoreOrder(){
    QString q="update 訂單 set 評分=" +ui->textEdit_8->toPlainText()+" , 是否評價=1 where ID="+ ui->textEdit_9->toPlainText() +";";
    QSqlQuery query;
    query.exec(q);
    QMessageBox msgBox;
    if(!query.isActive()) {
        msgBox.setText("評分失敗");
        msgBox.exec();
    } else {
        msgBox.setText("評分成功");
        msgBox.exec();
    }

}

void MainWindow::searchDetails() {
    ui->tableWidget_5->clear();
    ui->tableWidget_5->setColumnCount(0);
    ui->tableWidget_5->setRowCount(0);
    ui->tableWidget_6->clear();
    ui->tableWidget_6->setColumnCount(0);
    ui->tableWidget_6->setRowCount(0);

    QString q;
    if(ui->radioButton_6->isChecked()) {
        q="select 訂單ID, 品名, 數量, 價格,  數量*價格 as 小計 from 購物清單,商品 where 商品.ID=商品ID and 是否加入訂單=1 and 買家ID="+ui->textEdit_10->toPlainText()+" and 訂單ID in (select 訂單ID from 購物清單,商品 where 商品.ID=商品ID and 是否加入訂單=1 and 買家ID="+ui->textEdit_10->toPlainText()+" group by 訂單ID having SUM(數量*價格)>"+ui->textEdit_low->toPlainText()+" and SUM(數量*價格)<"+ui->textEdit_up->toPlainText()+");";
    } else {
        q = "select 訂單ID, 品名, 數量, 價格,  數量*價格 as 小計 from 購物清單,商品 where  商品.ID=商品ID and 是否加入訂單=1 and 買家ID="+ui->textEdit_10->toPlainText()+";";
    }
    QSqlQuery query;
    query.exec(q);
    QMessageBox msgBox;
    if(!query.size() || !query.isActive()) {
        msgBox.setText("查無資料");
        msgBox.exec();
        return;
    }

    QSqlRecord rec = query.record();
    QStringList header;

    for (int var = 0; var < rec.count(); ++var) {
        QString colName = rec.fieldName(var);
        header.append(colName);
    }
    // set table

    ui->tableWidget_5->setColumnCount(rec.count());
    ui->tableWidget_5->setRowCount(query.size());
    ui->tableWidget_5->setHorizontalHeaderLabels(header);

    int row=0;
    while(query.next())
    {
        int column=0;
        for(QString v:header) {
            QString value = query.value(v).toString();
            ui->tableWidget_5->setItem(row, column, new QTableWidgetItem(value));
            column++;
        }
        row++;
    }

    if(ui->radioButton_6->isChecked()) {
        q="select 訂單ID,COUNT(*) as 購物清單數量,SUM(數量*價格) as 總和 from 購物清單,商品 where  商品.ID=商品ID and 是否加入訂單=1 and 買家ID="+ui->textEdit_10->toPlainText()+" group by 訂單ID having 總和>"+ui->textEdit_low->toPlainText()+" and 總和<"+ui->textEdit_up->toPlainText()+";";
    } else {
        q="select 訂單ID,COUNT(*) as 購物清單數量,SUM(數量*價格) as 總和 from 購物清單,商品 where  商品.ID=商品ID and 是否加入訂單=1 and 買家ID="+ui->textEdit_10->toPlainText()+" group by 訂單ID;";
    }
    query.exec(q);
    if(!query.size() || !query.isActive()) {
        msgBox.setText("查無資料");
        msgBox.exec();
        return;
    }

    rec = query.record();
    header.clear();

    for (int var = 0; var < rec.count(); ++var) {
        QString colName = rec.fieldName(var);
        header.append(colName);
    }
    // set table

    ui->tableWidget_6->setColumnCount(rec.count());
    ui->tableWidget_6->setRowCount(query.size());
    ui->tableWidget_6->setHorizontalHeaderLabels(header);

    row=0;
    while(query.next())
    {
        int column=0;
        for(QString v:header) {
            QString value = query.value(v).toString();
            ui->tableWidget_6->setItem(row, column, new QTableWidgetItem(value));
            column++;
        }
        row++;
    }

    // Set MAX and min 訂單
    q[q.size()-1]=')';
    QString max="select MAX(總和) as 最高金額 from ("+q+" 訂單總和;";
    query.exec(max);
    while(query.next())
    {
            QString value = query.value("最高金額").toString();
            ui->label_max->setText(value);
    }
    QString min="select MIN(總和) as 最低金額 from ("+q+" 訂單總和;";
    query.exec(min);
    while(query.next())
    {
            QString value = query.value("最低金額").toString();
            ui->label_min->setText(value);
    }
    QString avg="select AVG(總和) as 平均金額 from ("+q+" 訂單總和;";
    query.exec(avg);
    while(query.next())
    {
            QString value = query.value("平均金額").toString();
            ui->label_avg->setText(value);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

