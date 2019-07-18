#include "clientmain.h"
#include "ui_clientmain.h"
#include "searchwindow.h"
#include "rootwindow.h"

ClientMain::ClientMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientMain)
{
    ui->setupUi(this);

    udpSocked = new QUdpSocket(this);
    connect(udpSocked,SIGNAL(readyRead()),this,SLOT(confirm_Slots()));

    QString ip = "192.168.10.184";
    QString port = "5004";
    serverip.setAddress(ip);
    serverport = port.toShort();

    mess.status = false;
}

ClientMain::~ClientMain()
{
    delete ui;
}

void ClientMain::on_changeButton_clicked()
{
    if(ui->changeButton->text() == "Normal"){
        ui->changeButton->setText("Root");
        mess.status = true;

    }else{
        ui->changeButton->setText("Normal");
        mess.status = false;
    }
}

void ClientMain::on_signButton_clicked()
{
    mess.user = ui->userEdit->text();
    mess.password = ui->passEdit->text();
    ui->userEdit->clear();
    ui->passEdit->clear();

    udpSocked->writeDatagram((char*)&mess,sizeof(mess),QHostAddress::LocalHost,serverport);
    ui->signButton->setEnabled(false);
}

void ClientMain::confirm_Slots(){
    char gram[2];
    if(udpSocked->hasPendingDatagrams()){
        udpSocked->readDatagram(gram,sizeof(gram));
    }
    ui->signButton->setEnabled(true);

    if(!(QString::compare(gram,"OK"))){
        if(mess.status){
            rootWindow* win = new rootWindow(mess.user);
            win->show();
        }else{
            searchWindow* win = new searchWindow(mess.user);
            win->show();
        }
        this->close();
    }else{
        QMessageBox::information(this,"error","password is wrong!");
    }
}
