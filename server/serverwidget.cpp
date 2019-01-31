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
#include <QMessageBox>
#include <QCheckBox>

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

    connect(tcpserverob,SIGNAL(newConnection()), SLOT(AcceptConnect()));   //服务端新的连接信号,触发槽函数，等待客户端连接

    //socket通信连接状态错误提示输出
    connect(tcpsocketob,SIGNAL(error(QAbstractSocket::SocketError)), SLOT(ShowError(QAbstractSocket::SocketError)));

    connect(ui->ServerDelpushButton,SIGNAL(clicked(bool)), SLOT(ClearRecvData())); //点击按钮，触发槽函数--清空TextEdit控件数据
}


//从服务端发送消息给客户端
void ServerWidget::SendMessage()
{
    QString sendata = ui->ServerSendDatalineEdit->text();  //获取lineEdit控件的输入内容

    //拼接字符串加上当前时间
    QString strdata = QString::fromLocal8Bit("Time:") + QTime::currentTime().toString() + '\n' + sendata.toLocal8Bit() + '\n';

    QByteArray sendmessage = strdata.toLocal8Bit();  // 转换字符串为字节数组 数据

   // mchat += ("Server Send:" + sendmessage);

   // ui->ServerReceDatatextEdit->setText(mchat);  //在本窗口的textEdit控件显示数据

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

    //是否选中 将字符串转为 十六进制 字节数组
    if(!ui->ServerHexcheckBox->isChecked())
    {
        qDebug() << "Text Format";
        mchat += ("Recv Client " + data);
        ui->ServerReceDatatextEdit->setText(mchat);

    }else {

        QByteArray strtohex = data.toUtf8().toHex().toUpper();   //将字符串转换为 十六进制数据，大写，支持中文

        QByteArray hextemp = strtohex.right(2);   //将十六进制字节数据末尾的两个字符提取出来

        int count = strtohex.length();  //获取字节数组长度

        QByteArray Showhex;  // 存储完整格式的 十六进制数据 内容

        //循环遍历字节数组,每次计数自增2， 每次提取2个字符
        //i=28是字节数组 前面的 《当前时间》 数据， 不用显示出来
        for(int i=28; i<count-2; i+=2)
          {

            QByteArray temp = strtohex.mid(i, 2);   //从字节数组i位置开始提取，每次提取2个字符

            //每2个字符加上一个空格
            Showhex += temp;
            Showhex += " ";

        }

        Showhex = Showhex + hextemp;  //拼接完整 的十六进制字节数据
        qDebug() << Showhex;
        ui->ServerReceDatatextEdit->append(Showhex);  //UI界面textedit控件显示 十六进制数据

}

    //检测是否选中--写入文件功能
    if(!ui->ServerCheckBox->isChecked())
    {
        qDebug() << "off";

    }else {

         qDebug() << "on";
        //写入文件--调试日志记录
        QFile *file = new QFile("ClentTest.txt");
        if(file->open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ))
        {
             QTextStream out(file);
             out << data;
             file->flush();
             file->close();
        }
        else {
              QMessageBox::warning(this, "warning", "open file error", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
              qDebug() << "open file error";
            }
}

    //mchat += ("Recv Client " + data);
   // ui->ServerReceDatatextEdit->setText(mchat);


}


//显示输出错误
void ServerWidget::ShowError(QAbstractSocket::SocketError)
{
    qDebug() << tcpsocketob->errorString();

    tcpsocketob->close();  //关闭socket
}


//清空接收到所有的数据
void ServerWidget::ClearRecvData()
{
    ui->ServerReceDatatextEdit->clear();   //清空TextEdit控件里显示的内容
    mchat = "";                           //清空mchat对象缓存的数据
}
