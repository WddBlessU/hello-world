#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new QSqlDatabase;
    usermodel = new QSqlQueryModel(this);
    historymodel = new QSqlQueryModel(this);
    model = new QSqlQueryModel();

    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("APP.db");
    db->open();

    QSqlQuery Query;
    Query.prepare("CREATE TABLE user(""id INT PRIMARY KEY NOT NULL,""name TEXT NOT NULL,""password TEXT NOT NULL,""sex TEXT NOT NULL,""age INT NOT NULL,""salary REAL NOT NULL,""level TEXT NOT NULL)");
    Query.exec();
    Query.prepare("CREATE TABLE history(""operate TEXT PRIMARY KEY NOT NULL)");
    Query.exec();

    showTable();

//    connect(this,SIGNAL(showDB(QSqlQueryModel* showModel)),main_addr,SLOT(showTable(QSqlQueryModel* showModel)));
//    emit showDB(historymodel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTable(){
    historymodel->setQuery("SELECT * FROM history");
    ui->tableView->setModel(historymodel);
    ui->tableView->show();
}

QSqlQueryModel* MainWindow::search(QString user,QString* pass){

    QString sortstr = QString("SELECT * FROM user ""WHERE name = '%1'").arg(user);

    QSqlQuery Query;
    Query.exec(sortstr);
    model->setQuery(sortstr);

    while(Query.next())
    {
        *pass = Query.value(2).toString();
    }
    return model;
}

void MainWindow::udpSearchSlots(QString user,QString* pass){
     qDebug()<< this;
    search(user,pass);
}
//---------------------------------------------------
udpOb::udpOb(void* m_addr)
{
    QString ip = "192.168.10.184";
    QString port = "5004";
    serverip.setAddress(ip);
    serverport = port.toShort();

    udpSocked = new QUdpSocket(this);
    udpSocked->bind(quint16(serverport));
    connect(udpSocked,SIGNAL(readyRead()),this,SLOT(confirmSlots()));
    connect(this,SIGNAL(udpsearch(QString,QString*)),(MainWindow*)m_addr,SLOT(udpSearchSlots(QString,QString*)));
}

void udpOb::confirmSlots(){
    LogMess mess;
    if(udpSocked->hasPendingDatagrams()){
        udpSocked->readDatagram((char*)&mess,sizeof(mess),&clientip,&clientport);
    }else{
        return;
    }

    QString pass;

    emit udpsearch(mess.user,&pass);

    qDebug()<<  "yes";

    if(!(QString::compare(pass,mess.password))){
        qDebug()<<  "yes";

        udpSocked->writeDatagram("OK",clientip,clientport);
    }else{
        qDebug()<<  "yes";

        udpSocked->writeDatagram("NO",clientip,clientport);
    }
}

tcpOb::tcpOb()
{
    QString ip = "192.168.10.184";
    QString port = "5004";
    serverip.setAddress(ip);
    serverport = port.toShort();

    tcpserver = new QTcpServer();
    tcpserver->listen(serverip,serverport);
//  connect(tcpserver,SIGNAL(newConnection()),this,SLOT(newConnection_Slots()));
}

void loginThread::run(){
    obj = new udpOb(main_addr);

//  connect(obj->udpSocked, SIGNAL(readyRead()),this,SLOT(confirmSlots()),Qt::DirectConnection);

    exec();
}

void serverThread::run(){
    obj = new tcpOb();
//  connect(obj->tcpserver,SIGNAL(newConnection()),this,SLOT(newConnection_Slots()),Qt::DirectConnection);

    exec();
}
/*
void serverThread::newConnection_Slots()
{
    QTcpSocket* tcpClientSocked = tcpserver->nextPendingConnection();
    tcpClientList.append(tcpClientSocked);
    connect(tcpClientSocked,SIGNAL(readyRead()),this,SLOT(messageRev_Slots()));
}

void serverThread::messageRev_Slots(){
    for(int i = 0;i < tcpClientList.count();i++){
        qDebug() << "socket counts：" << tcpClientList.count();
        qDebug() << "socket status：" << tcpClientList.at(i)->state();
        if(tcpClientList.at(i)->state() == QAbstractSocket::UnconnectedState){
            tcpClientList.removeAt(i);
        }
    }

    for(int i = 0;i < tcpClientList.count();i++){
        if(tcpClientList.at(i)->bytesAvailable()){
            QByteArray buf = tcpClientList.at(i)->readAll();

        }
    }
}
*/
