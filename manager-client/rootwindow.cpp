#include "rootwindow.h"
#include "ui_rootwindow.h"

struct person{
    int ID;
    QString name;
    QString password;
    QString sex;
    int age;
    float salary;
    QString level;
};

rootWindow::rootWindow(QString name, QWidget *parent) :
    keyname(name),QMainWindow(parent),
    ui(new Ui::rootWindow)
{
    ui->setupUi(this);

    qDebug()<< keyname << name;

    tcpsocked = new QTcpSocket(this);

    QString ip = "192.168.1.100";
    serverip.setAddress(ip);
    QString port = "8888";
    serverport = port.toShort();

    connect(tcpsocked,SIGNAL(readyRead()),this,SLOT(revMessage_Slots()));
    connect(tcpsocked,SIGNAL(connected()),this,SLOT(connected_Slots()));

    tcpsocked->connectToHost(serverip,serverport);
}

rootWindow::~rootWindow()
{
    delete ui;
}

void rootWindow::on_insertButton_clicked()
{
    sending sender;
    sender.style = ui->comboBox->currentText();
    sender.text = ui->lineEdit->text();
    ui->lineEdit->clear();
}

void rootWindow::on_changeButton_clicked()
{

}

void rootWindow::on_deleteButton_clicked()
{

}

void rootWindow::on_searchButton_clicked()
{

}

void rootWindow::connected_Slots()
{
    QMessageBox::information(this,"About","connect success!");
}

void rootWindow::revMessage_Slots()
{

}

