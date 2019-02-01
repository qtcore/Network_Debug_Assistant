#include "clientwidget.h"
#include <QApplication>
#include <QTranslator>
#include <QObject>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //创建国际化翻译 对象
     QTranslator appTranslator;
     appTranslator.load("myexec_zh_CN.qm"); //加载翻译过的.qm文件
     qApp->installTranslator(&appTranslator);  //在应用程序安装翻译 appTranslator对象

    ClientWidget w;

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    w.show();

    return a.exec();
}
