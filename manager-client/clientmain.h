#ifndef CLIENTMAIN_H
#define CLIENTMAIN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
#include <QUdpSocket>
#include "clientmain.h"

namespace Ui {
class ClientMain;
}

#pragma pack(1)
struct LogMess{
    bool status;
    QString user;
    QString password;
};
#pragma pack()

class ClientMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientMain(QWidget *parent = 0);
    ~ClientMain();

private slots:
    void on_changeButton_clicked();

    void on_signButton_clicked();

    void confirm_Slots();

private:
    Ui::ClientMain *ui;

    QUdpSocket *udpSocked;
    quint16 serverport;
    QHostAddress serverip;

    LogMess mess;
};


#endif // CLIENTMAIN_H

