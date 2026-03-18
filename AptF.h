#ifndef APTF_H
#define APTF_H

#include <QMainWindow>

#include "PreCompile.h" //KNotify

#include "PlainTextEditMouseRelease.h" //PlainTextEditMouseRelease
#include "AptfTtsClient.h" //AptfTtsClient
#include "AptfSettingDialog.h" //AptfSettingDialog

namespace Ui {
class AptF;
}

class AptF : public QMainWindow
{
    Q_OBJECT

public:
    explicit AptF(QWidget *parent = 0);
    ~AptF();
    
private:
    enum DoingWorkType
    {
        RpmInstall=1, //!<rpm安装。
        ServiceStart=2, //!<启动服务。
        ApkInstall=3, //!<APK安装。
        RpmBuild=4, //!<RPM打包。
        GemsInstall=5, //!<gems安装。
        PasswordCompress=6, //!<带密码压缩文件。
        RpmQuery=7, //!<当前的工作类型是，RPM查询。
        ServiceStop=8, //!<当前的工作类型是，服务停止。
        ServiceEnable=9, //!<当前的工作类型是，服务启用。
        GemsUninstall=10, //!<当前的工作类型是，红宝石卸载。
        RpmValidateWorkType=11, //!<Rpm validate.
        RpmRemove=12, //!<rpm卸载。
        ServiceDisable=13, //!<禁用服务。
    }; //enum DoingWorkType

    void reactToOutput(QString Otpt, QProcess *currentProcess); //!<对输出内容做出响应。
    bool httpProxyAddEnvironmentVariable=false; //!<是否要为http代理设置环境变量。
    QSet<QString> rpmValidateIgnoreSet; //!<The rpm validate output ignore set.
    bool processApplicationEventFilter(QEvent * event); //!<Process event filter about application object.
    void executeOperationRelatedLogic(QProcess * currentProcess); //!<Execute operation related logic.
    QString catCmd="cat"; //!<The cat command.
    QString findSplittedFileName(QStringList Pkg2IstLst); //!<Find the prefix name by part file name.
    QString splitCmd="split"; //!<The split command.
    QString sizeUnitToString(int uisplitSizeUnitcomboBoxcurrentIndex); //!<Convert unit to string.
    quint16 socksProxyPort; //!<socks5代理的端口号。
    void initializeFunctionIndexFunctionTabWidgetMap(); //!<初始化大功能标签页下标与大功能标签页部件之间的映射关系。
    QMap<int,QTabWidget*> functionIndexTabWidgetMap; //!<大功能标签页下标与大功能标签而之间的映射关系。
    QString getSubFunctionTitle(int currentFunctionIndex); //!<获取子功能标题。
    void resetProcessId(int extCode); //!<重置进程编号。
    void updateProcessId(); //!<更新进程编号。
    QLabel * processIdLabel; //!<进程编号文字标签。
    void connectSignals(); //!<连接信号槽。
    void initializeMemberObjects(); //!<初始化成员变量。
    void toastNotice(QString noticeText); //!<吐息报告。
    bool hasRunningProcess(); //!<还有子进程未退出。
    void AsesShow(QSystemTrayIcon::ActivationReason ActRsn); //!<点击左键时，显示主窗口。
    QMenu TrayMn; //!<托盘图标的菜单。
    void showTray(); //!<显示托盘图标。
    void loadSetting(); //!<载入设置信息。
    AptfSettingDialog stngDlg; //!<设置对话框。
    quint16 httpProxyPort; //!<http代理的端口号。
    AptfTtsClient ttsClient; //!<TTS客户端。
    void ttsSay(QString trRpminstallfinished); //!<使用TTS服务器来发出语音报告。
    DoingWorkType doingWorkType; //!<当前正在进行的工作的类型。
    QString systemctlCmd="systemctl"; //!<systemctl命令。
    QString adbCmd="adb"; //!<adb命令。
    QString serviceCmd="service"; //!<service命令。
    void installEventFilters(); //!<安装各个部件的事件过滤器。
    QString mountCmd="mount"; //!<mount命令。
    QString yumCmd="yum"; //!<yum.
    QString konsoleCmd="konsole"; //!<konsole.
    QString GemCmd="gem"; //!<gem的可执行程序。
    QAction * OpenCppAct=new QAction(tr("Open cpp"),this); //Make the action of open cpp file.
    QString AptCacheCmd="apt-cache"; //!<apt-cache command.
    QString UuidgenCmd="uuidgen"; //!<Uuidgen command.
    QAction * HbrInstAct=new QAction(tr("Homebrew install"),this); //Make the action of homebrew install.
    QAction * AptInstAct=new QAction(tr("Apt-get install"),this); //Make the action of apt-get install.
    QAction * AptRmvAct=new QAction(tr("Apt-get remove"),this); //!<Make the action of apt-get remove.
    QAction * rpmValidateIgnoreAction=new QAction(tr("Rpm validate ignore"),this); //!<The action of rpm validate ignore.
    QAction * rpmFindByFileAction=new QAction(tr("Rpm find by file"),this); //!<The action of rpm find by file.
    QAction * CpAct=new QAction(tr("Copy text"),this); //Make the action of copy text.
    QString RarCmd="rar"; //!<Rar command.
    QString BrewCmd="/usr/local/bin/brew"; //!<The command line of brew.
    QAction * OpnUrlAct=new QAction(tr("Open url"),this); //Make the action of open url.
    PlainTextEditMouseRelease * OtptTxtEdt; //!<The output text edit.
    QLabel * ActLbl; //!<进度栏上显示当前事件的文字标签。
    QSystemTrayIcon TrayIcn; //!<托盘图标。
    QString RpmBuildCmd="rpmbuild"; //!<rpmbuild命令。
    bool FilterOutput=false; //!<是否应当过滤输出内容。
    QString GrepCmd="grep"; //!<grep命令行。
    QProcess GrepPrc; //!<grep进程对象。
    QProcess CatPrc; //!<cat process object.
    QString RpmCmd="rpm"; //!<rpm命令行。
    QTextDecoder * StdErrdecoder; //!<标准错误流的字符编码解码器。
    QTextDecoder * decoder; //!<字符编码解码器。
    QProcess AptPrc; //!<用于启动apt-get的进程对象。
    QString AptCmd="apt-get"; //!<apt-get命令行。
    Ui::AptF *ui;

private slots:
    void adbDevices(); //!<列出adb设备列表。
    void generateFileListWildcard(); //!<生成文件列表的通配符列表。
    void rpmRemove(); //!<RPM卸载。
    void gemUninstall(); //!<卸载红宝石。
    void konsoleLaunch(); //!<启动konsole.
    void yumProvides(); //!<使用yum来查询哪个包提供了某个文件.
    void catMergeFiles(); //!<使用cat来merge file.
    void splitFile(); //!<使用split来split file.
    void gemContents(); //!<列出某个gem的内容。
    void updateWindowTitle(); //!<更新窗口标题。
    void gemLocalInstall(); //!<gem本地安装。
    void enableService(); //!<启用服务，即，使该服务开机自动启动。
    void disableService(); //!<禁用服务。
	void stopService(); //!<停止服务。
    void quit(); //!<退出。
    void saveSetting(); //!<设置对话框确认关闭，则保存设置信息。
    void showSettingDialog(); //!<显示设置对话框。
    void noticeFinished(int extCode); //!<命令成功退出，则语音报告。
    void killProcess(); //!<杀死进程。
    void adbInstall(); //!<adb安装apk.
    void checkServiceStatus(); //!<检查服务的状态。
    void restartService(); //!<重新启动服务。
    void startService(); //!<启动服务。
    void mountDevice(); //!<挂载设备文件。
    void srpmRebuild(); //!<rebuild by src.rpm.
    void yumLocalInstall(); //!<yum local install.
    void GemInstall(); //!<使用gem来安装软件包。
    void RpmValidate(); //!<Rpm validate the package.
    void OpenCpp(); //!<Open the cpp file.
    void BrewLink(); //!<Homebrew link package.
    void JumpToError(); //!<Jump to the last error.
    void AptCacheSearch(); //!<Search available packages by apt-cache.
    void ChangeBrewFocus(int CrtTb); //!<Change the focus of controls in the brew tab.
    void CopyText(); //!<The copy text action is triggered,then copy text.
    void Uuidgen(); //!<Uuidgen.Generate uuid.
    void BrewRemove(); //!<Homebrew remove package.
    void BrewInstall(); //!<Homebrew install.
    void RarPasswordCompress(); //!<Rar compress with password.
    void ReportError(QProcess::ProcessError AptErr); //!<Error ocurred,then report error.
    void HomebrewSearch(); //!<Use homebrew to search for a package.
    void KateEdit(); //!<Use kate to edit the spec file.
    void OpenUrl(); //!<The open url action is triggered,then open the url.
    void generateContextMenu(); //!<Generate the context menu.
    void rpmFindPackageByFile(); //!<按照文件路径来寻找对应的软件包。
    void rpmValidateIgnore(); //!<Add an ignore item for the rpm validate.
    void Notify(int ExtCd); //!<apt命令退出，则弹出提示。
    void ListFile(); //!<列出RPM包中的文件。
    void rpmBuild(); //!<打包RPM。
    void EnableButtons(int ExtCd); //!<apt命令退出，则重新启用各个按钮。
    void DisableButtons(); //!<禁用按钮们。
    void LocalInstall(); //!<安装本地RPM包。
    void QueryInfo(); //!<查询RPM包信息。
    void ListInstalledPackages(); //!<列出已经安装的软件包。
    void DistUpgrade(); //!<升级发行版。
    void Update(); //!<更新信息。
    void AutoFillPkgName(); //!<自动填充软件包名。
    void ShowAptOutputStdErr(); //!<apt-get有标准错误流输出，则显示。
    void showProcessOutput(); //!<apt-get有内容输出，则显示。
    void Install(); //!<安装。
    void Remove(); //!<删除。
    void Upgrade(); //!<升级。

protected:
    bool eventFilter ( QObject * watched, QEvent * event ); //!<事件过滤器。
};

#endif // APTF_H
