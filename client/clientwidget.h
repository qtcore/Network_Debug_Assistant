#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QTimer>
#include <QDebug>
#include <QByteArray>


namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();


public slots:
    void init();
    void ConnectServer(); //连接到服务端
private slots:
    void SendMessage();  //发送消息给服务端
    void RecvData();     //接收来自服务端的数据
    void ShowError(QAbstractSocket::SocketError);  //输出错误
    void ClearRecvData();     //清空TextEdit控件接收到的数据

private:
    Ui::ClientWidget *ui;

    QTcpSocket *clientsocket;
    QByteArray mchat;

};

#endif // CLIENTWIDGET_H
