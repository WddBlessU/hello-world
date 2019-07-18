#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>

namespace Ui {
class searchWindow;
}

class searchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit searchWindow(QString name,QWidget *parent = 0);
    ~searchWindow();

private slots:
    void on_pushButton_clicked();

    void revMessage_Slots();

    void connected_Slots();

private:
    Ui::searchWindow *ui;

    QTcpSocket *tcpsocked;
    quint16 serverport;
    QHostAddress serverip;

    QString keyname;
};

#endif // SEARCHWINDOW_H
