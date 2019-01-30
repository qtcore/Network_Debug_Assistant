#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QByteArray>
#include <QDebug>
#include <QTimer>


namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

public:
    void init();          //初始化对象

private slots:
    void SendMessage();   //发送消息
    void RecvData();      //接收数据
    void NewLinten();     //建立TCP监听事件
    void AcceptConnect(); //等待客户端连接
    void ShowError(QAbstractSocket::SocketError); //显示Socket连接状态错误输出
    void ClearRecvData();     //清空TextEdit控件接收到的数据



private:
    Ui::ServerWidget *ui;

    QTcpServer *tcpserverob;
    QTcpSocket *tcpsocketob;
    QByteArray mchat;

};

#endif // SERVERWIDGET_H
