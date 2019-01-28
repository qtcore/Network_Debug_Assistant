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


ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromLocal8Bit("Client"));

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
}

void ClientWidget::SendMessage()
{
    QString sendata = ui->ClinentSendDatalineEdit->text();
    QString strdata = QString::fromLocal8Bit("Time:") + QTime::currentTime().toString() + '\n'+ sendata.toLocal8Bit() + '\n';

    QByteArray sendmessage = strdata.toLocal8Bit();

    mchat += ("Client Send" + sendmessage);
    ui->ClinentRecvDatatextEdit->setText(mchat);

    clientsocket->write(sendmessage);
}

//建立连接到服务端
void ClientWidget::ConnectServer()
{
        QString clientip = ui->ClientIPlineEdit->text(); //获取IP
        quint16 clientport = ui->ClientPortlineEdit->text().toUShort(); //获取端口号

        clientsocket->abort();
        clientsocket->connectToHost(QHostAddress(clientip),clientport); //建立连接
        ui->ConnpushButton->setText("conneted ok");


}




void ClientWidget::RecvData()
{
    QString recvdata = clientsocket->readAll();
    //QString hexstr = recvdata.toUtf8().toHex().toUpper();
    qDebug() << recvdata;

    mchat += ("Recv Server "+ recvdata);
    ui->ClinentRecvDatatextEdit->setText(mchat);

}


void ClientWidget::ShowError(QAbstractSocket::SocketError)
{
    qDebug() << clientsocket->errorString();
    clientsocket->close();
}

