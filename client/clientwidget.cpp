#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>
#include <QTranslator>
#include <QObject>


ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{

    ui->setupUi(this);

    setWindowTitle(QString(tr("Client")));

    init();

    connect(ui->ClinentSendDatapushButton,SIGNAL(clicked(bool)), SLOT(SendMessage()));

}

ClientWidget::~ClientWidget()
{
    delete ui;
}


//初始化
void ClientWidget::init()
{
    clientsocket = new QTcpSocket;  //创建客户端socket对象

    connect(ui->ConnpushButton,SIGNAL(clicked(bool)), SLOT(ConnectServer())); //点击连接按钮，连接到服务端

    connect(clientsocket,SIGNAL(readyRead()), SLOT(RecvData()));   //成功连接后，接收来自服务端的数据
     connect(clientsocket,SIGNAL(error(QAbstractSocket::SocketError)), SLOT(ShowError(QAbstractSocket::SocketError)));

     connect(ui->ClientDelpushButton,SIGNAL(clicked(bool)), SLOT(ClearRecvData()));   //点击按钮，触发槽函数-清空数据

}

void ClientWidget::SendMessage()
{
    QString sendata = ui->ClinentSendDatalineEdit->text();
    QString strdata = QString::fromLocal8Bit("Time:") + QTime::currentTime().toString() + '\n'+ sendata.toLocal8Bit() + '\n';

    QByteArray sendmessage = strdata.toLocal8Bit();

  //  mchat += ("Client Send" + sendmessage);
   // ui->ClinentRecvDatatextEdit->setText(mchat);

    clientsocket->write(sendmessage);
}

//建立连接到服务端
void ClientWidget::ConnectServer()
{
        QString clientip = ui->ClientIPlineEdit->text(); //获取IP
        quint16 clientport = ui->ClientPortlineEdit->text().toUShort(); //获取端口号

        clientsocket->abort();
        clientsocket->connectToHost(QHostAddress(clientip),clientport); //建立连接
        ui->ConnpushButton->setText(tr("conneted ok"));


}




void ClientWidget::RecvData()
{
    QString recvdata = clientsocket->readAll();
    //QString hexstr = recvdata.toUtf8().toHex().toUpper();
   // QString temp2 = recvdata.left(14);
   // qDebug() << temp2;

    //是否选中字符串转换为十六进制 字节数组 功能
    if(!ui->ClientHexcheckBox->isChecked())
    {
        qDebug() << "Text Format";

        //字节数组显示数据
        mchat += ("Recv Server "+ recvdata);
        ui->ClinentRecvDatatextEdit->setText(mchat);

    }else{

        QByteArray strtoHex = recvdata.toUtf8().toHex().toUpper();   //转换字符串为十六进制字节数组，大写，支持中文


        QByteArray strhextemp = strtoHex.right(2);    // 将字节数组末尾的两个字符提取出来

        //获取字节数组的长度
        int count = strtoHex.length();
        //qDebug() << count; //在控制台显示 字节长度

        QByteArray ShowHex;  //存储 转换 完整格式的 十六进制字节数组  数据内容

        //循环读取字节数组的字符，每次读取两个字符
        //count-2是因为已经提取了字节数组末尾的两个字符出去
        //i=28是 获取当前时间的 字节数，转换十六进制不需要转换 时间
        for(int i=28; i<count-2; i+=2)
        {
            QByteArray temp = strtoHex.mid(i, 2);      //在字节数组的i位置开始提取字符，每次提取两个字符

            ShowHex += temp;  //拼接字符数组，每2个字符加上一个空格
            ShowHex += " ";
        }

        ShowHex = ShowHex + strhextemp;  //拼接完整的字节数组 数据
        qDebug() << ShowHex;
        ui->ClinentRecvDatatextEdit->append(ShowHex);
    }


    //检测是否选中--写入文件功能
    if(!ui->ClientCheckBox->isChecked())
    {
        qDebug() << "off";

    }else {

         qDebug() << "on";
        //写入文件--调试日志记录
        QFile *file = new QFile("ClentTest.txt");
        if(file->open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ))
        {
             QTextStream out(file);
             out << recvdata;
             file->flush();
             file->close();
        }
        else {
              QMessageBox::warning(this, "warning", "open file error", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
              qDebug() << "open file error";
            }
}



    //字节数组显示数据
   // mchat += ("Recv Server "+ recvdata);
   // ui->ClinentRecvDatatextEdit->setText(mchat);

}


void ClientWidget::ShowError(QAbstractSocket::SocketError)
{
    qDebug() << clientsocket->errorString();
    clientsocket->close();
}


//清空TextEdit控件接收到的数据
void ClientWidget::ClearRecvData()
{
    ui->ClinentRecvDatatextEdit->clear();  //清除TextEdit控件内容
    mchat = "";                            //清除mchat对象缓存的数据
}
