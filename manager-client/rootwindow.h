#ifndef ROOTWINDOW_H
#define ROOTWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>

namespace Ui {
class rootWindow;
}

struct sending{
    QString style;
    QString text;
};

class rootWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit rootWindow(QString name,QWidget *parent = 0);
    ~rootWindow();

private slots:
    void on_insertButton_clicked();

    void on_changeButton_clicked();

    void on_deleteButton_clicked();

    void on_searchButton_clicked();

    void revMessage_Slots();

    void connected_Slots();

private:
    Ui::rootWindow *ui;

    QTcpSocket *tcpsocked;
    quint16 serverport;
    QHostAddress serverip;

    QString keyname;
};

#endif // ROOTWINDOW_H
