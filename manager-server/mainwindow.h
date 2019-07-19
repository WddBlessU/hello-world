#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QThread>

namespace Ui {
class MainWindow;
}

#pragma pack(1)
struct LogMess{
    bool status;
    QString user;
    QString password;
};
#pragma pack()

struct person{
    int ID;
    QString name;
    QString password;
    QString sex;
    int age;
    float salary;
    QString level;
};
//-----------------------------------MainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showTable();

    QSqlQueryModel* search(QString user,QString* pass);

public slots:
    void udpSearchSlots(QString user,QString* pass);

private:
    Ui::MainWindow *ui;

    QSqlDatabase* db;
    QSqlQueryModel* usermodel;
    QSqlQueryModel* historymodel;
    QSqlQueryModel* model;
};
//-----------------------------------udp server
class udpOb : public QObject{
    Q_OBJECT

public:
    udpOb(void* m_addr);

signals:
    void udpsearch(QString user,QString* pass);

public slots:
    void confirmSlots();

private:
    QUdpSocket* udpSocked;
    QHostAddress serverip;
    QHostAddress clientip;
    quint16 serverport;
    quint16 clientport;
};
//-----------------------------------tcp server
class tcpOb : public QObject{
    Q_OBJECT

public:
    tcpOb();
    void callEmitSignal(){
        emit someSignal();
    }

signals:
    void someSignal();

public slots:
//    void newConnection_Slots();

//    void messageRev_Slots();

private:
    QTcpServer *tcpserver;
    QHostAddress serverip;
    quint16 serverport;

    QList <QTcpSocket*> tcpClientList;
};
//-----------------------------------log thread
class loginThread : public QThread{
    Q_OBJECT

public:
    loginThread(void* addr,QObject* parent=0) :main_addr(addr),QThread(parent){}
    virtual ~loginThread(){
        if (obj!=NULL) delete obj;
    }

    void run();

public:
    udpOb* obj;
    void* main_addr;
};
//-----------------------------------server thread
class serverThread : public QThread{
    Q_OBJECT

public:
    serverThread(QObject* parent=0) : QThread(parent){}
    virtual ~serverThread(){
        if (obj!=NULL) delete obj;
    }

    void run();

public:
    tcpOb* obj;
};

#endif // MAINWINDOW_H
