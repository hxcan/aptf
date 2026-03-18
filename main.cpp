#include "AptF.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed (false); //关闭窗口的情况下还不退出。

    a.setApplicationName(QObject::tr("AptF")); //设置程序名字。
    a.setQuitOnLastWindowClosed (false); //关闭窗口的情况下还不退出。

    QTranslator XyTrns;
    bool TrnsLdRst=XyTrns.load(":/AptF"); //载入翻译文件。

    qDebug() << __FILE__ << __LINE__ << __func__ << QObject::tr("Translation load result:") << TrnsLdRst << QObject::tr("Application dir path:") << QApplication::applicationDirPath(); //Debug.

    a.installTranslator (&XyTrns); //安装翻译文件。

    QCoreApplication::setOrganizationName("StupidBeauty"); //设置组织名字。
    QCoreApplication::setOrganizationDomain("stupidbeauty.com"); //设置组织域名。
    QCoreApplication::setApplicationName("AptF"); //设置程序名字。


    AptF w;
    w.show();
    
    return a.exec();
}
