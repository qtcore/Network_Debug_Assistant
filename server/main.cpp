#include "serverwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWidget w;

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    w.show();

    return a.exec();
}
