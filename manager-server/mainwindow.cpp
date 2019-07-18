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

    createDB();
    showTable();

    lThread = new loginThread;
    sThread = new serverThread;
    lThread->start();
    sThread->start();

    qDebug()<< this;
    qDebug()<< sThread;
    qDebug()<< lThread;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createDB(void){
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("APP.db");
    bool ok = db->open();
    if(ok == false){
        QMessageBox::critical(this,"Error",db->lastError().text());
        return;
    }

    QSqlQuery Query;
    Query.prepare("CREATE TABLE user(""id INT PRIMARY KEY NOT NULL,""name TEXT NOT NULL,""password TEXT NOT NULL,""sex TEXT NOT NULL,""age INT NOT NULL,""salary REAL NOT NULL,""level TEXT NOT NULL)");
    Query.exec();
    Query.prepare("CREATE TABLE history(""operate TEXT PRIMARY KEY NOT NULL)");
    Query.exec();
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

void loginThread::run(){

    work();

    while(1){
        QThread::msleep(100);
    }
}

void loginThread::work(){
    QString ip = "192.168.10.184";
    QString port = "5004";
    serverip.setAddress(ip);
    serverport = port.toShort();

    udpSocked = new QUdpSocket(this);
    udpSocked->bind(quint16(serverport));
    connect(udpSocked,SIGNAL(readyRead()),this,SLOT(confirmSlots()));
}

void loginThread::confirmSlots(){
    LogMess mess;
    if(udpSocked->hasPendingDatagrams()){
        udpSocked->readDatagram((char*)&mess,sizeof(mess),&clientip,&clientport);
    }else{
        return;
    }

    QString pass;
    MainWindow temp;
    temp.search(mess.user,&pass);
    if(!(QString::compare(pass,mess.password))){
        udpSocked->writeDatagram("OK",clientip,clientport);
    }else{
        udpSocked->writeDatagram("NO",clientip,clientport);
    }
}

void serverThread::run(){
    QString ip = "192.168.10.184";
    QString port = "5004";
    serverip.setAddress(ip);
    serverport = port.toShort();

    tcpserver = new QTcpServer();
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(newConnection_Slots()));

    tcpserver->listen(serverip,serverport);
    while(1){
        QThread::msleep(100);
    }
}

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
