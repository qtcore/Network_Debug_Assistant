#include "serverwidget.h"
#include <QApplication>
#include <QTabWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //加载国际化 中文翻译
    QTranslator appTranslator;
    appTranslator.load("myexec_cn.qm");
    qApp->installTranslator(&appTranslator);

    ServerWidget w;

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));



    w.show();



    return a.exec();
}
