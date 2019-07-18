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

class loginThread : public QThread
{
    Q_OBJECT

protected:
    void run();

private slots:
   void confirmSlots();

private:
    void work();

private:
    quint16 serverport;
    quint16 clientport;
    QHostAddress serverip;
    QHostAddress clientip;
    QUdpSocket* udpSocked;
};

class serverThread : public QThread
{
    Q_OBJECT

protected:
    void run();

private slots:
    void newConnection_Slots();

    void messageRev_Slots();

private:
    quint16 serverport;
    QHostAddress serverip;
    QTcpServer *tcpserver;

    QList <QTcpSocket*> tcpClientList;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void createDB();
    void showTable();
    QSqlQueryModel* search(QString user,QString* pass);

private:
    Ui::MainWindow *ui;

    QSqlDatabase* db;
    QSqlQueryModel* usermodel;
    QSqlQueryModel* historymodel;
    QSqlQueryModel* model;

    loginThread* lThread;
    serverThread* sThread;
};

#endif // MAINWINDOW_H
