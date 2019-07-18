#include "searchwindow.h"
#include "ui_searchwindow.h"

struct person{
    int ID;
    QString name;
    QString password;
    QString sex;
    int age;
    float salary;
    QString level;
};

searchWindow::searchWindow(QString name, QWidget *parent) :
    keyname(name),QMainWindow(parent),
    ui(new Ui::searchWindow)
{
    ui->setupUi(this);

    tcpsocked = new QTcpSocket(this);

    QString ip = "192.168.1.100";
    serverip.setAddress(ip);
    QString port = "8888";
    serverport = port.toShort();

    connect(tcpsocked,SIGNAL(connected()),this,SLOT(connected_Slots()));
    connect(tcpsocked,SIGNAL(readyRead()),this,SLOT(revMessage_Slots()));

    tcpsocked->connectToHost(serverip,serverport);
}

searchWindow::~searchWindow()
{
    delete ui;
}

void searchWindow::on_pushButton_clicked()
{
    QString msg = ui->lineEdit->text();
    ui->lineEdit->clear();
    tcpsocked->write(msg.toLocal8Bit());
}

void searchWindow::connected_Slots()
{
    QMessageBox::information(this,"About","connect success!");
}

void searchWindow::revMessage_Slots()
{

}
