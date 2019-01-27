#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <QWidget>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QByteArray>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QObject>
#include <QString>
#include <QTextEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromLocal8Bit("Server")); //设置窗口标题

    init();

    connect(ui->ServerSendDatapushButton,SIGNAL(clicked(bool)), SLOT(SendMessage())); //从服务端发送消息给客户端

}

ServerWidget::~ServerWidget()
{
    delete ui;
}


void ServerWidget::init()
{
    tcpserverob = new QTcpServer; //创建服务对象

    tcpsocketob = new QTcpSocket; //创建服务通讯对象

    connect(ui->ListenpushButton,SIGNAL(clicked(bool)),SLOT(NewLinten())); //点击监听按钮，触发监听

    connect(tcpserverob,SIGNAL(newConnection()), SLOT(AcceptConnect()));   //服务端连接信号,触发槽函数，等待客户端连接

    //socket通信连接状态错误提示输出
    connect(tcpsocketob,SIGNAL(error(QAbstractSocket::SocketError)), SLOT(ShowError(QAbstractSocket::SocketError)));
}


//从服务端发送消息给客户端
void ServerWidget::SendMessage()
{
    QString sendata = ui->ServerSendDatalineEdit->text();  //获取lineEdit控件的输入内容

    //拼接字符串加上当前时间
    QString strdata = QString::fromLocal8Bit("Time:") + QTime::currentTime().toString() + '\n' + sendata.toLocal8Bit() + '\n';

    QByteArray sendmessage = strdata.toLocal8Bit();  // 转换字符串为字节数组 数据

    mchat += ("Server Send:" + sendmessage);

    ui->ServerReceDatatextEdit->setText(mchat);  //在本窗口的textEdit控件显示数据

    tcpsocketob->write(sendmessage);   //向socket对象 写入字节数组 数据


}

//监听IP和端口号 事件
void ServerWidget::NewLinten()
{
    QString serverarrip = ui->ServerIPlineEdit->text(); //设置IP地址
    quint16 serverport = ui->ServerPortlineEdit->text().toUShort(); //设置端口号

    //如果没有监听到
    if(!tcpserverob->listen(QHostAddress(serverarrip),serverport))
    {
        qDebug() <<  tcpserverob->errorString(); //输出错误

        tcpserverob->close(); //关闭服务

    }
    else{
           ui->ListenpushButton->setText("LintenOk");
           qDebug() << "linten ok";
    }
}


//等待客户端连接
void ServerWidget::AcceptConnect()
{
    tcpsocketob = tcpserverob->nextPendingConnection();   //服务端阻塞，等待客户端连接
    connect(tcpsocketob,SIGNAL(readyRead()), SLOT(RecvData()));  //通过socket对象发送读取数据信号，触发槽函数--读取客户端数据

}


//接收来自客户端的数据
void ServerWidget::RecvData()
{
    QString data = tcpsocketob->readAll();  // 通过socket读取来自客户端的所有数据

    mchat += ("Recv Client " + data);
    ui->ServerReceDatatextEdit->setText(mchat);


}


//显示输出错误
void ServerWidget::ShowError(QAbstractSocket::SocketError)
{
    qDebug() << tcpsocketob->errorString();

    tcpsocketob->close();  //关闭socket
}

