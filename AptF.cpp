#include "PreCompile.h" //apt

#include "AptF.h"
#include "ui_AptF.h"

/*!
 * \brief AptF::initializeMemberObjects 初始化成员变量。
 */
void AptF::initializeMemberObjects()
{
    OtptTxtEdt=new PlainTextEditMouseRelease(this); //Make the output text edit.
    OtptTxtEdt->setMaximumBlockCount (10000); //10000 blocks max.
    OtptTxtEdt->setAcceptDrops(false); //Not accept drop.
    QFont BgFnt=OtptTxtEdt->font(); //Get the font.
    BgFnt.setPixelSize(20); //Set the font size.dddd/opt/kde4/bin/kwin &
    OtptTxtEdt->setFont(BgFnt); //Set the font.
    ui->centralWidget->layout()->addWidget(OtptTxtEdt); //Add to the layout.

    TrayIcn.setIcon (QIcon(":/Images/003_002.jpg")); //设置托盘图标的图片。

    TrayIcn.show(); //显示托盘图标。

    ui->PkgNmlineEdit->setFocus(); //焦点放到软件包名字输入框中。

    QTextCodec * gbk_codec=QTextCodec::codecForLocale(); //文字解码器。
    decoder=gbk_codec->makeDecoder(); //获取解码器。
    StdErrdecoder=gbk_codec->makeDecoder(); //获取解码器。


    ActLbl=new QLabel; //创建进度文字标签。
    ui->statusBar->addPermanentWidget(ActLbl); //添加当前进度的文字标签。


    processIdLabel=new QLabel; //创建进程编号文字标签。
    processIdLabel->setText(tr("Process id:%1").arg (tr("unknown"))); //显示文字。
    ui->statusBar->addPermanentWidget (processIdLabel); //添加文字标签。

    initializeFunctionIndexFunctionTabWidgetMap(); //初始化大功能标签页下标与大功能标签页部件之间的映射关系。


    return;
} //void AptF::initializeMemberObjects()

/*!
 * \brief AptF::connectSignals 连接信号槽。
 */
void AptF::connectSignals()
{
    connect(OtptTxtEdt,SIGNAL(MouseReleased()),this,SLOT(generateContextMenu())); //Mouse released in the output text edit ,then generate context menu.
    connect(OtptTxtEdt,SIGNAL(selectionChanged()),this,SLOT(AutoFillPkgName())); //Selection changed , then auto fill package name.


    connect(OpnUrlAct,SIGNAL(triggered()),this,SLOT(OpenUrl())); //The open url action is triggered,then open the url.
    connect(AptInstAct,SIGNAL(triggered()),this,SLOT(Install())); //The apt-get install action is triggered,then apt-get install.
    connect(AptInstAct,SIGNAL(triggered()),this,SLOT(DisableButtons())); //The apt-get install action is triggered,then disable buttons.
    connect(AptRmvAct,SIGNAL(triggered()),this,SLOT(Remove())); //The apt-get remove action is triggered,then apt-get remove.
    connect(rpmFindByFileAction,&QAction::triggered,this,&AptF::rpmFindPackageByFile); //Should find by file, then find by file.
    connect(rpmValidateIgnoreAction,&QAction::triggered,this,&AptF::rpmValidateIgnore); //Should find by file, then find by file.

    connect(CpAct,SIGNAL(triggered()),this,SLOT(CopyText())); //The copy text action is triggered,then copy text.
    connect(HbrInstAct,SIGNAL(triggered()),this,SLOT(BrewInstall())); //The homebrew install action is triggered,then homebrew install.
    connect(HbrInstAct,SIGNAL(triggered()),this,SLOT(DisableButtons())); //The homebrew install action is triggered,then disable buttons.
    connect(OpenCppAct,SIGNAL(triggered()),this,SLOT(OpenCpp())); //The open cpp action is triggered,then open the cpp file.
    connect (&stngDlg,SIGNAL(accepted()),this,SLOT(saveSetting())); //设置对话框确认关闭，则保存设置信息。

    connect (&TrayIcn,SIGNAL(messageClicked()),this,SLOT(raise())); //单击消息，则显示主界面。
    connect (&TrayIcn,&QSystemTrayIcon::activated,this,&AptF::AsesShow); //点击左键时，显示主窗口。

    connect(&AptPrc,SIGNAL(readyRead()),this,SLOT(showProcessOutput())); //apt-get有内容输出，则显示。
    connect(&AptPrc,&QProcess::started,this,&AptF::updateProcessId); //进程启动，则更新进程编号。
    connect(&GrepPrc,SIGNAL(readyRead()),this,SLOT(showProcessOutput())); //grep有内容输出，则显示。
    connect(&AptPrc,SIGNAL(readyReadStandardError()),this,SLOT(ShowAptOutputStdErr())); //apt-get有标准错误流输出，则显示。
    connect(&AptPrc,SIGNAL(finished(int)),this,SLOT(EnableButtons(int))); //apt命令退出，则重新启用各个按钮。
    connect(&AptPrc,SIGNAL(finished(int)),this,SLOT(Notify(int))); //apt命令退出，则弹出提示。
    connect(&AptPrc, (void (QProcess::*)(int,QProcess::ExitStatus))&QProcess::finished,this,&AptF::noticeFinished); //命令成功退出，则报告。
    connect(&AptPrc, (void (QProcess::*)(int,QProcess::ExitStatus))&QProcess::finished,this,&AptF::resetProcessId); //命令成功退出，则重置进程编号。
    connect(&AptPrc,SIGNAL(error(QProcess::ProcessError)),this,SLOT(ReportError(QProcess::ProcessError))); //Error ocurred,then report error.

    connect(ui->PkgNmlineEdit->lineEdit(),SIGNAL(returnPressed()),this,SLOT(Install())); //编辑完成，则开始安装软件包。
    connect(ui->PkgNmlineEdit->lineEdit(),SIGNAL(returnPressed()),this,SLOT(DisableButtons())); //编辑完成，则禁用各个按钮。
    connect(ui->PkgNmlineEdit_3->lineEdit(),SIGNAL(returnPressed()),this,SLOT(HomebrewSearch())); //编辑完成，则Start to homebrew search。
    connect(ui->PkgNmlineEdit_8->lineEdit(),SIGNAL(returnPressed()),this,SLOT(AptCacheSearch())); //Edit finished,then start apt-cache search.
    connect(ui->PkgNmlineEdit_5->lineEdit(),SIGNAL(returnPressed()),this,SLOT(BrewInstall())); //Edit finished,then homebrew install.
    connect(ui->PkgNmlineEdit_5->lineEdit(),SIGNAL(returnPressed()),this,SLOT(DisableButtons())); //Edit finished,then disable buttons.
    connect(ui->PkgNmlineEdit_7->lineEdit(),SIGNAL(returnPressed()),this,SLOT(BrewLink())); //Edit finished,then brew link.


    return;
} //void AptF::connectSignals()

/*!
 * \brief AptF::AptF 构造函数。
 * \param parent 亲代对象。
 */
AptF::AptF(QWidget *parent) :QMainWindow(parent),ui(new Ui::AptF)
{
    ui->setupUi(this);


    initializeMemberObjects(); //初始化成员变量。

    connectSignals(); //连接信号槽。






    installEventFilters(); //安装各个部件的事件过滤器。




    showTray(); //显示托盘图标。

    loadSetting(); //载入设置信息。

} //AptF::AptF(QWidget *parent) :QMainWindow(parent),ui(new Ui::AptF)

/*!
 * \brief AptF::~AptF Destructor.
 */
AptF::~AptF()
{
    ui->FlNmlineEdit_5->lineEdit()->removeEventFilter(this); //删除事件过滤器。

    delete StdErrdecoder; //Delete the decoder.
    delete decoder; //Delete the decoder.

    delete ui;
} //AptF::~AptF()

/*!
 * \brief AptF::Install 安装。
 */
void AptF::Install()
{
    ui->ApttabWidget->setCurrentWidget(ui->Isttab_3); //Switch to the install tab.
    ui->FunctabWidget_2->setCurrentWidget(ui->Apttab); //Switch to apt-get tab.

    FilterOutput=false; //不过滤输出内容。

    //获取软件包的名字，再调用apt来安装。

    //获取软件包名字：
    QString Pkg2Ist=ui->PkgNmlineEdit->currentText().trimmed(); //获取软件包名字。

    QStringList Pkg2IstLst=Pkg2Ist.split(" "); //分割字符串。

    Pkg2IstLst.removeAll(""); //删除空白的字符串实例。

    //安装：
    QStringList AptPrms; //命令行参数。
    AptPrms << "install" << Pkg2IstLst; //加入软件包名字。
    AptPrc.start(AptCmd,AptPrms); //启动进程。
} //void AptF::Install() //!<安装。

/*!
 * \brief AptF::ShowAptOutput apt-get有内容输出，则显示。
 */
void AptF::showProcessOutput()
{
    bool actualOutputtedText=false; //Actual outputted text.
    QProcess * currentProcess=(QProcess *)sender(); //获取发送数据的进程的指针。

    while (currentProcess->bytesAvailable()) //Still has content to read.
    {
        QString Otpt=decoder->toUnicode(currentProcess->readAllStandardOutput()); //读入所有输出。
        
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.

        if (FilterOutput) //要过滤输出内容。
        {
            //将输出内容分割成一行行的：
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
            QStringList OtptStrs=Otpt.split('\n'); //分割。

            QString CrtStr=""; //当前行的字符串。
            foreach(CrtStr,OtptStrs) //一行行地处理。
            {
                cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                if (CrtStr.trimmed().isEmpty()) //It is an empty line.
                {

                } //if (CrtStr.trimmed().isEmpty()) //It is an empty line.
                else //Have text.
                {
                    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                    if (CrtStr.contains(ui->GrplineEdit->text())) //包含了要过滤的内容。
                    {
                        OtptTxtEdt->appendPlainText(CrtStr); //显示。

                        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                        actualOutputtedText=true; //Actual outputted text.
                    } //if (Otpt.contains(ui->GrplineEdit->text())) //包含了要过滤的内容。

                } //else //Have text.
                cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.

            } //foreach(CrtStr,OtptStrs) //一行行地处理。
        } //if (FilterOutput) //要过滤输出内容。
        else //不过滤输出内容。
        {
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
            if (doingWorkType==RpmValidateWorkType)
            {
                //将输出内容分割成一行行的：
                QStringList OtptStrs=Otpt.split('\n'); //分割。
                cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.

                QString CrtStr=""; //当前行的字符串。

                foreach(CrtStr,OtptStrs) //一行行地处理。
                {
                    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                    if (rpmValidateIgnoreSet.contains(CrtStr)) //包含了要过滤的内容。
                    {
                    } //if (Otpt.contains(ui->GrplineEdit->text())) //包含了要过滤的内容。
                    else //No need to filter.
                    {
                        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                        if (CrtStr.trimmed().isEmpty()) //It is an empty line.
                        {

                        }
                        else //Have text.
                        {
                            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                            OtptTxtEdt->appendPlainText(CrtStr); //显示。

                            actualOutputtedText=true; //Actual outputted text.
                        }
                        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                    } //else //No need to filter.
                } //foreach(CrtStr,OtptStrs) //一行行地处理。
            } //if (doingWorkType==RpmValidateWorkType)
            else //Not doing rpm validate.
            {
                cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                if (Otpt.trimmed().isEmpty()) //It is an empty line.
                {

                }
                else {
                    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                    OtptTxtEdt->appendPlainText(Otpt); //显示。

                    actualOutputtedText=true; //Actual outputted text.

                    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
                }

            } //else //Not doing rpm validate.
        } //else //不过滤输出内容。
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        
        
        //针对输出内容做出响应：
        reactToOutput(Otpt, currentProcess); //对输出内容做出响应。

        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        if (actualOutputtedText)
        {
            executeOperationRelatedLogic(currentProcess); //Execute operation related logic.
        } //if (actualOutputtedText)
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
    } //while (CrtPrc->bytesAvailable()) //Still has content to read.
} //void AptF::ShowAptOutput() //!<apt-get有内容输出，则显示。

/*!
 * \brief AptF::reactToOutput 对输出内容做出响应。
 */
void AptF::reactToOutput(QString Otpt, QProcess * currentProcess)
{
    
    //Get:1 http://jaist.dl.sourceforge.net update/soft qt4-doc 4.8.4-2mgc26 [134MB]
    QRegExp GetRpmReg("Get:[0-9]\\s.*\\s.*\\s(.*)\\s.*\\s\\[[0-9]*(\\.)*[0-9]*M|kB\\]"); //表示当前正在下载RPM包的输出语句。

    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
    if (Otpt.contains("Do you want to continue? [Y/n]")) //有询问语句。
    {
        qDebug() << __FILE__ << __LINE__ << __func__ ; //Debug.
        QMessageBox::StandardButton Ctn=QMessageBox::question(this,tr("AptF question"),tr("Please read the output and decide whether we should continue the operation"),QMessageBox::Yes | QMessageBox::No); //询问。
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.

        if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        {
            currentProcess->write("Y\n"); //输入Y。
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        } //if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        else //拒绝继续操作。
        {
            currentProcess->write("n\n"); //输入n。
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        } //else //拒绝继续操作。
    } //if (Otpt.contains("Do you want to continue? [Y/n]")) //有询问语句。
    else if (Otpt.contains("Press Enter to start installation or ^C to abort")) //有询问语句。针对firebird打包。在安装阶段会这样询问。
    {
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        qDebug() << __FILE__ << __LINE__ << __func__ ; //Debug.
        currentProcess->write("\n"); //输入回车。
    } //if (Otpt.contains("Do you want to continue? [Y/n]")) //有询问语句。
    else if (Otpt.contains("Press return to continue or ^C to abort")) //有询问语句。针对firebird打包。在安装阶段会这样询问。
    {
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        QMessageBox::StandardButton Ctn=QMessageBox::question(this,tr("AptF question"),tr("Please read the output and decide whether we should continue the operation"),QMessageBox::Yes | QMessageBox::No); //询问。

        if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        {
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
            currentProcess->write("\n"); //输入回车。
        } //if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        else //拒绝继续操作。
        {
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
            //此时应当按Ctrl+C.

            ::kill(currentProcess->processId (),SIGINT); //发送信号。
        } //else //拒绝继续操作。
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
    } //if (Otpt.contains("Do you want to continue? [Y/n]")) //有询问语句。
    else if (Otpt.contains("replace FreeMind.exe? [y]es, [n]o, [A]ll, [N]one, [r]ename:")) //有询问语句。针对freemind打包。如果rpmbuild未清空会这样。
    {
        QMessageBox::StandardButton Ctn=QMessageBox::question(this,tr("AptF question"),tr("Please read the output and decide whether we should continue the operation"),QMessageBox::Yes | QMessageBox::No); //询问。
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.

        if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        {
            currentProcess->write("A\n"); //输入回车。
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        } //if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        else //拒绝继续操作。
        {
            currentProcess->write("N\n"); //输入回车。
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        } //else //拒绝继续操作。
    } //if (Otpt.contains("Do you want to continue? [Y/n]")) //有询问语句。
    else if (Otpt.contains("replace espeak-1.48.04-source/ChangeLog.txt? [y]es, [n]o, [A]ll, [N]one, [r]ename:")) //有询问语句。针对freemind打包。如果rpmbuild未清空会这样。
    {
        QMessageBox::StandardButton Ctn=QMessageBox::question(this,tr("AptF question"),tr("Please read the output and decide whether we should continue the operation"),QMessageBox::Yes | QMessageBox::No); //询问。
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.

        if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        {
            currentProcess->write("A\n"); //输入回车。
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        } //if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        else //拒绝继续操作。
        {
            currentProcess->write("N\n"); //输入回车。
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        } //else //拒绝继续操作。
    } //if (Otpt.contains("Do you want to continue? [Y/n]")) //有询问语句。
    else if (Otpt.contains("replace AUTHORS? [y]es, [n]o, [A]ll, [N]one, [r]ename:")) //有询问语句。针对bazel打包。如果rpmbuild未清空会这样。
    {
        QMessageBox::StandardButton Ctn=QMessageBox::question(this,tr("AptF question"),tr("Please read the output and decide whether we should continue the operation"),QMessageBox::Yes | QMessageBox::No); //询问。
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.

        if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        {
            currentProcess->write("A\n"); //输入回车。
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        } //if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        else //拒绝继续操作。
        {
            currentProcess->write("N\n"); //输入回车。
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        } //else //拒绝继续操作。
    } //if (Otpt.contains("Do you want to continue? [Y/n]")) //有询问语句。
    else if (Otpt.contains("To continue type in the phrase 'Yes, do as I say!'")) //有询问语句。
    {
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        QMessageBox::StandardButton Ctn=QMessageBox::warning(this,tr("AptF warning"),tr("You're going to do something really harmful.Please read the output and decide whether we should continue the operation"),QMessageBox::Yes | QMessageBox::No); //询问。

        if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        {
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
            currentProcess->write("Yes, do as I say!\n"); //输入Yes, do as I say!。
        } //if (Ctn==QMessageBox::Yes) //要继续操作，则输入Y。
        else //拒绝继续操作。
        {
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
            currentProcess->write("n\n"); //输入n。
        } //else //拒绝继续操作。
    } //else if (Otpt.contains("To continue type in the phrase 'Yes, do as I say!'")) //有询问语句。
    else if (Otpt.contains(GetRpmReg)) //有正在下载信息语句。Get:1 http://jaist.dl.sourceforge.net update/soft qt4-doc 4.8.4-2mgc26 [134MB]
    {
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        qDebug() << __FILE__ << __LINE__ << __func__ << tr("Downloading info got:") << GetRpmReg.cap(1); //Debug.

        ActLbl->setText(GetRpmReg.cap(1)); //显示正在下载的软件包名字。
    } //else if () //有正在下载信息语句。Get:1 http://jaist.dl.sourceforge.net update/soft qt4-doc 4.8.4-2mgc26 [134MB]
    else if (Otpt.contains("Default answer will be chosen in 120 seconds...")) //Fink needs making decision.
    {
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
        bool isOK;
        int text = QInputDialog::getInt(this, "Decision needed","Please read the output and decide which option to choose and enter the corresponding number below",1,1,7,1,&isOK);
        if(isOK) //Pressed OK.
        {
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
            currentProcess->write(tr("%1\n").arg(text).toUtf8()); //输入the number。
        } //if(isOK) //Presse
        else //Pressed cancel.
        {
            cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
            currentProcess->write("\n"); //Just press return.
        } //else //Pressed cancel.
    } //else if (Otpt.contains("Default answer will be chosen in 120 seconds...")) //Fink needs making decision.
    
} //void AptF::reactToOutput()

/*!
 * \brief AptF::executeOperationRelatedLogic Execute operation related logic.
 * \param currentProcess The working process.
 */
void AptF::executeOperationRelatedLogic(QProcess * currentProcess)
{
    if (doingWorkType==RpmValidateWorkType) //Rpm validate.
    {
        if (ui->quickStopcheckBox->isChecked()) //Should stop as quickly as possible.
        {
            ::kill(currentProcess->processId (),SIGINT); //发送信号。
        } //if (ui->quickStopcheckBox->isChecked()) //Should stop as quickly as possible.
    } //if (doingWorkType==RpmValidate) //Rpm validate.

} //void AptF::executeOperationRelatedLogic(QProcess * currentProcess)

/*!
 * \brief AptF::ShowAptOutputStdErr apt-get有标准错误流输出，则显示。
 */
void AptF::ShowAptOutputStdErr()
{
    QString Otpt=StdErrdecoder->toUnicode(AptPrc.readAllStandardError()); //读入所有输出。

    OtptTxtEdt->appendPlainText(Otpt); //显示。

    //如果其中有询问语句，则弹出询问对话框：
    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Otpt:") << Otpt; //Debug.
    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", output: " << Otpt.toStdString() << endl; //Debug.
    
    QProcess * currentProcess=&AptPrc;
    
    reactToOutput(Otpt, currentProcess); //对输出内容做出响应。
} //void AptF::ShowAptOutputStdErr() //!<apt-get有标准错误流输出，则显示。

/*!
 * \brief AptF::Upgrade 升级。
 */
void AptF::Upgrade()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "upgrade"; //加入升级命令。
    AptPrc.start(AptCmd,AptPrms); //启动进程。
} //void AptF::Upgrade() //!<升级。

/*!
 * \brief AptF::Remove 删除。
 */
void AptF::Remove()
{
    DisableButtons(); //Disable buttons.

    ui->ApttabWidget->setCurrentWidget(ui->AptRmvtab_5); //Toggle to apt-get remove tab.
    ui->FunctabWidget_2->setCurrentWidget(ui->Apttab); //Toggle to apt-get tab.

    //获取软件包名字：
    QString Pkg2Ist=ui->PkgNmlineEdit_2->text().trimmed(); //获取软件包名字。

    QStringList Pkg2IstLst=Pkg2Ist.split(" "); //分割字符串。

    FilterOutput=false; //不过滤输出内容。

    //删除：
    QStringList AptPrms; //命令行参数。
    AptPrms << "remove" << Pkg2IstLst; //加入软件包名字。
    AptPrc.start(AptCmd,AptPrms); //启动进程。

    return;
} //void AptF::Remove() //!<删除。

/*!
 * \brief AptF::AutoFillPkgName 自动填充软件包名。
 */
void AptF::AutoFillPkgName()
{
    QString selectedText=OtptTxtEdt->textCursor().selectedText(); //Get the selected text.

    if (ui->PkgNmlineEdit->findText(selectedText) == -1) //现在没有这个条目。
    {
        ui->PkgNmlineEdit->addItem(selectedText); //填充到软件包名字输入框中。
        ui->PkgNmlineEdit->setCurrentIndex(ui->PkgNmlineEdit->count()-1); //显示最后一条。
    } //if (ui->PkgNmlineEdit->findText(ui->OtptplainTextEdit->textCursor().selectedText()) == -1) //现在没有这个条目。
    else //有这个条目。
    {
        ui->PkgNmlineEdit->setCurrentIndex(ui->PkgNmlineEdit->findText(selectedText)); //显示与选中的内容对应的条目。
    } //else //有这个条目。

    ui->QryInfolineEdit->setText(selectedText); //填充软件包名字到查询信息输入框中。

    ui->PkgNmlineEdit_2->setText(selectedText); //填充到软件包名字输入框中。
    ui->RpmPkgNmlineEdit->setText(selectedText); //填充到软件包名字输入框中。

    ui->PkgNmlineEdit_5->lineEdit()->setText(selectedText); //Fill the package name.

    ui->FilelineEdit_2->setText(selectedText); //File in the file name.
} //void AptF::AutoFillPkgName() //!<自动填充软件包名。

/*!
 * \brief AptF::Update 更新信息。
 */
void AptF::Update()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "update"; //加入更新命令。
    AptPrc.start(AptCmd,AptPrms); //启动进程。

    return;
} //void AptF::Update() //!<更新信息。

/*!
 * \brief AptF::DistUpgrade 升级发行版。
 */
void AptF::DistUpgrade()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "dist-upgrade"; //加入升级发行版命令。
    AptPrc.start(AptCmd,AptPrms); //启动进程。

    return;
} //void AptF::DistUpgrade() //!<升级发行版。

/*!
 * \brief AptF::ListInstalledPackages 列出已经安装的软件包。
 */
void AptF::ListInstalledPackages()
{

    QStringList AptPrms; //命令行参数。
    AptPrms << "-qa"; //加入列出软件包列表命令。

    if (ui->GrplineEdit->text().isEmpty()) //不过滤软件包名字。
    {

    } //if (ui->GrplineEdit->text().isEmpty()) //不过滤软件包名字。
    else //过滤软件包名字。
    {
        FilterOutput=true; //应当过滤输出内容。
    } //else //过滤软件包名字。


    AptPrc.start(RpmCmd,AptPrms); //启动进程。


    return;
} //void AptF::ListInstalledPackages() //!<列出已经安装的软件包。

/*!
 * \brief AptF::QueryInfo 查询RPM包信息。
 */
void AptF::QueryInfo()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=RpmQuery; //当前的工作类型是，RPM查询。

    QStringList AptPrms; //命令行参数。
    AptPrms << "-qi"; //加入查询软件包信息命令。

    AptPrms << ui->QryInfolineEdit->text(); //加入软件包名字。


    AptPrc.start(RpmCmd,AptPrms); //启动进程。
} //void AptF::QueryInfo() //!<查询RPM包信息。

/*!
 * \brief AptF::generateFileListWildcard 生成文件列表的通配符列表。
 */
void AptF::generateFileListWildcard()
{
    QString fileListContent=ui->rawFileListplainTextEdit->toPlainText(); //获取全部内容。
    
    QStringList filePathList=fileListContent.split("\n");
    
    QSet<QString> wildCardFilePathSet; //最终的通配符路径集合。
    
    for(QString currentFilePath: filePathList) //一行行地处理。
    {
        currentFilePath=currentFilePath.trimmed(); //切掉首尾空白。
        
        QFileInfo currentFileInfo(currentFilePath);
        
        QString currentFileDirPath=currentFileInfo.absolutePath();
        
        QString currentWildcardFilePath=currentFileDirPath+"/*";
        
        if (wildCardFilePathSet.contains(currentWildcardFilePath)) //已经包含。
        {
            
        } //if (wildCardFilePathSet.contains(currentWildcardFilePath)) //已经包含。
        else //尚未包含。
        {
            wildCardFilePathSet << currentWildcardFilePath; //加入集合中。
        } //else //尚未包含。
    } //for(QString currentFilePath: filePathList) //一行行地处理。
    
    QString wildCardResult;
    
    for(QString currentWildCardPath: wildCardFilePathSet) //一行行地输出。
    {
        wildCardResult.append(currentWildCardPath+"\n");
    } //for(QString currentWildCardPath: wildCardFilePathSet) //一行行地输出。
    
    OtptTxtEdt->appendPlainText("\n");
    OtptTxtEdt->appendPlainText(wildCardResult);
    
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(wildCardResult);
    
} //void AptF::generateFileListWildcard()

/*!
 * \brief AptF::rpmRemove RPM卸载。
 */
void AptF::rpmRemove()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=RpmRemove; //当前的工作类型是，RPM卸载。

    QStringList AptPrms; //命令行参数。
    AptPrms << "-ve"; //加入卸载软件包的命令。详细输出。卸载软件。

    if (ui->nodepscheckBox_2->isChecked ()) //忽略依赖关系。
    {
        AptPrms << "--nodeps" ; //加入选项，忽略依赖关系。
    } //if (ui->nodepscheckBox->isChecked ()) //忽略依赖关系。


    QStringList Pkg2IstLst=ui->RpmlineEdit_2->text().trimmed().split("\n"); //分割字符串。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Pkg2IstLst:") << Pkg2IstLst; //Debug.


    AptPrms << Pkg2IstLst; //加入软件包名字。


    AptPrc.start(RpmCmd,AptPrms); //启动进程。
} //void AptF::rpmRemove()

/*!
 * \brief AptF::LocalInstall 安装本地RPM包。
 */
void AptF::LocalInstall()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=RpmInstall; //当前的工作类型是，RPM安装。

    QStringList AptPrms; //命令行参数。
    AptPrms << "--force" << "-vUh"; //加入安装软件包的命令。强制安装。

    if (ui->nodepscheckBox->isChecked ()) //忽略依赖关系。
    {
        AptPrms << "--nodeps" ; //加入选项，忽略依赖关系。
    } //if (ui->nodepscheckBox->isChecked ()) //忽略依赖关系。


    QStringList Pkg2IstLst=ui->RpmlineEdit->text().trimmed().split("\n"); //分割字符串。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Pkg2IstLst:") << Pkg2IstLst; //Debug.


    AptPrms << Pkg2IstLst; //加入软件包名字。


    AptPrc.start(RpmCmd,AptPrms); //启动进程。

    return;
} //void AptF::LocalInstall() //!<安装本地RPM包。

/*!
 * \brief AptF::DisableButtons 禁用按钮们。
 */
void AptF::DisableButtons()
{
    ui->LstIstdPckgscommandLinkButton_6->setEnabled(false); //禁用按钮。列出本地已经安装的软件包。
    ui->IstlcommandLinkButton->setEnabled(false); //禁用按钮。安装。
    ui->RmvcommandLinkButton_3->setEnabled(false); //禁用按钮。删除。
    ui->UpgrdcommandLinkButton_2->setEnabled(false); //禁用按钮，升级。
    ui->InstcommandLinkButton_8->setEnabled(false); //禁用按钮，本地安装。
    ui->SearchpushButton_2->setEnabled(false); //Disable the button,homebrew search.
    ui->IstpushButton_4->setEnabled(false); //Disable the homebrew install button.
    ui->BuildcommandLinkButton_6->setEnabled(false); //Disable the rpm build button.
    ui->VldtcommandLinkButton_11->setEnabled(false); //Disable the rpm validate button.
    ui->RmvcommandLinkButton_3->setEnabled(false); //Disable the apt-get remove button.
    ui->FndPkgByFlcommandLinkButton_10->setEnabled(false); //禁用按钮，按照文件寻找软件包。
    ui->IstlcommandLinkButton_3->setEnabled (false); //禁用按钮，gems安装。
    ui->adbInstallpushButton_5->setEnabled (false); //禁用按钮，APK安装。
    ui->gemLocalInstcommandLinkButton_9->setEnabled (false); //禁用按钮，gem安装。

    return;
} //void AptF::DisableButtons() //!<禁用按钮们。

/*!
 * \brief AptF::EnableButtons apt命令退出，则重新启用各个按钮。
 * \param ExtCd 命令退出代码。
 */
void AptF::EnableButtons(int ExtCd)
{
    (void)(ExtCd); //暂时不使用勒个参数。

    ui->LstIstdPckgscommandLinkButton_6->setEnabled(true); //启用按钮。列出本地已经安装的软件包。
    ui->IstlcommandLinkButton->setEnabled(true); //启用按钮。安装按钮。
    ui->RmvcommandLinkButton_3->setEnabled(true); //启用按钮。删除。
    ui->UpgrdcommandLinkButton_2->setEnabled(true); //启用按钮。升级。
    ui->InstcommandLinkButton_8->setEnabled(true); //启用按钮，本地安装。
    ui->SearchpushButton_2->setEnabled(true); //Enable the button,homebrew search.
    ui->IstpushButton_4->setEnabled(true); //Enable the homebrew install button.
    ui->BuildcommandLinkButton_6->setEnabled(true); //Enable the rpm build button.
    ui->VldtcommandLinkButton_11->setEnabled(true); //Enable the rpm validate button.
    ui->RmvcommandLinkButton_3->setEnabled(true); //Enable the apt-get remove button.
    ui->FndPkgByFlcommandLinkButton_10->setEnabled(true); //启用按钮，按照文件寻找软件包。
    ui->IstlcommandLinkButton_3->setEnabled (true); //启用按钮，gems安装。
    ui->adbInstallpushButton_5->setEnabled (true); //启用按钮，APK安装。
    ui->gemLocalInstcommandLinkButton_9->setEnabled (true); //启用按钮，gem安装。

    return;
} //void AptF::EnableButtons(int ExtCd) //!<apt命令退出，则重新启用各个按钮。

/*!
  *@brief 事件过滤器。
  *@param watched 被过滤的事件的所有者。
  *@param event 事件。
  *
  *这个函数主要是用来在弹出的绘图部件被单击时隐藏它。
  */
bool AptF::eventFilter ( QObject * watched, QEvent * event )
{
    bool Result=false; //结果。

    if (watched==ui->RpmlineEdit) //目标是本地RPM包文件路径输入框。
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            ui->RpmlineEdit->clear(); //清空。
        } //if

        Result=QObject::eventFilter(watched,event); //调用亲代类的事件过滤器。
    } //if
    else if (watched==ui->SpeclineEdit) //目标是spec文件路径输入框。
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            QDropEvent * DrEvt=(QDropEvent*)event; //转换成扔下事件。

            QString Url=DrEvt->mimeData()->text(); //获取URL。

            ui->SpeclineEdit->setText(DrEvt->mimeData()->urls()[0].path()); //填充路径。

            Result=true; //拦截事件。
        } //if (event->type()==QEvent::Drop) //是扔下文字事件。
    } //else if (watched==ui->SpeclineEdit) //目标是spec文件路径输入框。
    else if (watched==ui->adbFileNamelineEdit) //目标是apk文件路径输入框。
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            QDropEvent * DrEvt=(QDropEvent*)event; //转换成扔下事件。

            QString Url=DrEvt->mimeData()->text(); //获取URL。

            ui->adbFileNamelineEdit->setText(DrEvt->mimeData()->urls()[0].path()); //填充路径。

            Result=true; //拦截事件。
        } //if (event->type()==QEvent::Drop) //是扔下文字事件。
    } //else if (watched==ui->SpeclineEdit) //目标是spec文件路径输入框。
    else if (watched==ui->FilelineEdit_2) //目标是 the file of some package 输入框。
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            QDropEvent * DrEvt=(QDropEvent*)event; //转换成扔下事件。

            QString Url=DrEvt->mimeData()->text(); //获取URL。

            ui->FilelineEdit_2->setText(DrEvt->mimeData()->urls()[0].path()); //填充路径。

            Result=true; //拦截事件。
        } //if (event->type()==QEvent::Drop) //是扔下文字事件。
    } //else if (watched==ui->SpeclineEdit) //目标是spec文件路径输入框。
    else if (watched== ui->FlNmlineEdit_5->lineEdit()) //It is the rar file name.
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            QDropEvent * DrEvt=(QDropEvent*)event; //转换成扔下事件。

            QString Url=DrEvt->mimeData()->text(); //获取URL。

            ui->FlNmlineEdit_5->setEditText(DrEvt->mimeData()->urls()[0].path()); //填充路径。
            ui->RarNmlineEdit_6->setEditText(DrEvt->mimeData()->urls()[0].path()+".rar"); //Set the rar file name.

            Result=true; //拦截事件。
        } //if (event->type()==QEvent::Drop) //是扔下文字事件。
    } //else if (watched== ui->FlNmlineEdit_5) //It is the rar file name.
    else if (watched== ui->catFileNamePkgNmlineEdit_13->lineEdit()) //It is the cat file name.
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            QDropEvent * DrEvt=(QDropEvent*)event; //转换成扔下事件。

            QString Url=DrEvt->mimeData()->text(); //获取URL。

            QList<QUrl> urls=DrEvt->mimeData()->urls(); //Get the url list.

            QString pathList; //The path list.

            foreach(auto currentUrl,urls) //Add urls one by one.
            {
                QString currentPath=currentUrl.path(); //Get the path.

                pathList += currentPath+"\n"; //Add the current path.
            } //foreach(auto currentUrl,urls) //Add urls one by one.


            ui->catFileNamePkgNmlineEdit_13->setEditText(pathList); //Set the cat file name list.

            Result=true; //拦截事件。
        } //if (event->type()==QEvent::Drop) //是扔下文字事件。
    } //else if (watched== ui->FlNmlineEdit_5) //It is the rar file name.
    else if (watched== ui->gemLocalRpmlineEdit_2) //gem文件名。
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            QDropEvent * DrEvt=(QDropEvent*)event; //转换成扔下事件。

            QString Url=DrEvt->mimeData()->text(); //获取URL。

            QList<QUrl> urlList=DrEvt->mimeData ()->urls (); //获取URL列表。

            QString pathsString=urlList[0].path (); //路径字符串。


            ui->gemLocalRpmlineEdit_2->setText (pathsString); //填充路径。

            Result=true; //拦截事件。
        } //if (event->type()==QEvent::Drop) //是扔下文字事件。
    } //else if (watched== ui->FlNmlineEdit_5) //It is the rar file name.
    else if (watched== ui->fileNamePkgNmlineEdit_12->lineEdit ()) //split文件名。
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            QDropEvent * DrEvt=(QDropEvent*)event; //转换成扔下事件。

            QString Url=DrEvt->mimeData()->text(); //获取URL。

            QList<QUrl> urlList=DrEvt->mimeData ()->urls (); //获取URL列表。

            QString pathsString=urlList[0].path (); //路径字符串。


            ui->fileNamePkgNmlineEdit_12->setEditText (pathsString); //填充路径。

            Result=true; //拦截事件。
        } //if (event->type()==QEvent::Drop) //是扔下文字事件。
    } //else if (watched== ui->FlNmlineEdit_5) //It is the rar file name.
    else if (watched==ui->FlNmlineEdit_7->lineEdit ()) //目标是被挂载的设备文件路径输入框。
    {
        if (event->type()==QEvent::Drop) //是扔下文字事件。
        {
            QDropEvent * DrEvt=(QDropEvent*)event; //转换成扔下事件。

            QString Url=DrEvt->mimeData()->text(); //获取URL。

            ui->FlNmlineEdit_7->setEditText(DrEvt->mimeData()->urls()[0].path()); //填充路径。

            Result=true; //拦截事件。
        } //if (event->type()==QEvent::Drop) //是扔下文字事件。
    } //else if (watched==ui->SpeclineEdit) //目标是spec文件路径输入框。
    else if (watched==qApp) //It is the Application itself.
    {
        Result=processApplicationEventFilter(event); //Process event filter about application object.

    } //else if (watched==ui->OtptplainTextEdit) //It is the output plain text edit.
    else //不处理事件。
    {
        Result=QObject::eventFilter(watched,event); //调用亲代类的事件过滤器。
    } //else //不处理事件。

    return Result;
} //bool Good::eventFilter ( QObject * watched, QEvent * event )

/*!
 * \brief AptF::processApplicationEventFilter Process event filter about application object.
 * \param event The event object.
 * \return Whether we should consume the event.
 */
bool AptF::processApplicationEventFilter(QEvent * event)
{
    bool result=false;

    if (event->type()==QEvent::MouseButtonRelease) //Mouse up.
    {
        QMouseEvent * MsEvt=(QMouseEvent*)event; //Cast to a mouse event.

        if (OtptTxtEdt->rect().contains(MsEvt->pos())) //The mouse is inside the output text edit.
        {
            generateContextMenu(); //Generate context menu.

            result=true; //Block the event.
        } //if (ui->OtptplainTextEdit->rect().contains(MsEvt->pos())) //The mouse is inside the output text edit.
    } //if (event->type()==QEvent::MouseButtonRelease) //Mouse up.

    return result;
} //bool AptF::processApplicationEventFilter(QEvent * event)

/*!
 * \brief AptF::Build 打包RPM。
 */
void AptF::rpmBuild()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=RpmBuild; //正在做RPM打包。

    QStringList AptPrms; //命令行参数。

    if (ui->i686checkBox_2->isChecked()) //build for i686.
    {
        AptPrms << "--target=i686"; //target is i686.
    } //if (ui->i686checkBox->isChecked()) //build for i686.

    AptPrms << "-bb"; //加入安装软件包的命令。

    AptPrms << ui->SpeclineEdit->text(); //加入spec文件路径。

    if (httpProxyAddEnvironmentVariable) //需要添加代理的环境变量。
    {
        QProcessEnvironment env=QProcessEnvironment::systemEnvironment();

        QString proxyString=QString("http://127.0.0.1:%1").arg(httpProxyPort);

        env.insert("HTTP_PROXY",proxyString);
        AptPrc.setProcessEnvironment(env);
    } //if (httpProxyAddEnvironmentVariable) //需要添加代理的环境变量。

    AptPrc.start(RpmBuildCmd,AptPrms); //启动进程。
} //void AptF::Build() //!<打包RPM。



/*!
 * \brief AptF::ListFile 列出RPM包中的文件。
 */
void AptF::ListFile()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "-ql"; //加入安装软件包的命令。

    AptPrms << ui->RpmPkgNmlineEdit->text(); //加入软件包名字。


    AptPrc.start(RpmCmd,AptPrms); //启动进程。
} //void AptF::ListFile() //!<列出RPM包中的文件。

/*!
 * \brief AptF::Notify apt命令退出，则弹出提示。
 * \param ExtCd 退出代码。
 */
void AptF::Notify(int ExtCd)
{
    (void)(ExtCd); //暂时不使用勒个参数。

    TrayIcn.showMessage (tr("AptF"),tr("Command finished.")); //显示一个气泡。
} //void AptF::Notify(int ExtCd) //!<apt命令退出，则弹出提示。

/*!
 * \brief AptF::FindPkgByFile 按照文件路径来寻找对应的软件包。
 */
void AptF::rpmFindPackageByFile()
{
    ui->FunctabWidget_2->setCurrentWidget(ui->rpmtab_2);
    ui->RpmtabWidget_3->setCurrentWidget(ui->findPackageByFiletab_11);

    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "-qf" ; //加入寻找文件的软件包的命令。

    AptPrms << ui->FilelineEdit_2->text(); //加入文件路径。

    AptPrc.start(RpmCmd,AptPrms); //启动进程。
} //void AptF::FindPkgByFile() //!<按照文件路径来寻找对应的软件包。

/*!
 * \brief AptF::rpmValidateIgnore Add an ignore item for the rpm validate.
 */
void AptF::rpmValidateIgnore()
{
    QString SltdTxt=OtptTxtEdt->textCursor().selectedText(); //Get the selected text.

    rpmValidateIgnoreSet << SltdTxt; //Add to the ignore set.
} //void AptF::rpmValidateIgnore()

/*!
 * \brief AptF::GenerateContextMenu Generate the context menu.
 */
void AptF::generateContextMenu()
{
    QString SltdTxt=OtptTxtEdt->textCursor().selectedText(); //Get the selected text.

    QUrl MbAUrl(SltdTxt); //Maybe it's a url.

    QMenu * CtxtMn=new QMenu(this); //Make a menu.

    CtxtMn->addAction(AptInstAct); //Add the action of apt-get install.
    CtxtMn->addAction(CpAct); //Add the action of copy.
    CtxtMn->addAction(HbrInstAct); //Add the action of homebrew install.
    CtxtMn->addAction(AptRmvAct); //Add the action of apt-get remove.
    CtxtMn->addAction(rpmFindByFileAction); //Add the action of rpm find by file.
    CtxtMn->addAction(rpmValidateIgnoreAction); //Add the action of rpm validate ignore.

    if (SltdTxt.startsWith("http://") || SltdTxt.startsWith("https://")) //It is a valid url.
    {
        CtxtMn->addAction(OpnUrlAct); //Add the open url action.
    } //if (MbAUrl.isValid()) //It is a valid url.

    if (SltdTxt.endsWith(".cpp")) //It is a cpp file path.
    {
        CtxtMn->addAction(OpenCppAct); //Add the open cpp action.
    } //if (SltdTxt.endsWith(".cpp")) //It is a cpp file path.

    if (CtxtMn->isEmpty()) //The menu is empty.
    {
    } //if (MbAUrl.isEmpty()) //The menu is empty.
    else //The menu is not empty.
    {
        if (SltdTxt.isEmpty()) //No text selected.
        {
        } //if (SltdTxt.isEmpty()) //No text selected.
        else //Some text is selected.
        {
            QPoint cursorPosition=OtptTxtEdt->cursor().pos();
            QPoint menuPosition=cursorPosition;

            QScreen * currentScreen=qApp->primaryScreen();

            int screenHeight=currentScreen->geometry().height();

            if ((cursorPosition.y() +  CtxtMn->height()) > (screenHeight)) //Too low.
            {
                menuPosition.setY(cursorPosition.y()-CtxtMn->height());
            } //if ((cursorPosition.y() +  CtxtMn->height()) > (screenHeight))

            CtxtMn->move(menuPosition); //Set the position.

            CtxtMn->show(); //Show the menu.
        } //else //Some text is selected.
     } //else //The menu is not empty.
} //void AptF::GenerateContextMenu() //!<Generate the context menu.

/*!
 * \brief AptF::OpenUrl The open url action is triggered,then open the url.
 */
void AptF::OpenUrl()
{
    QString SltdTxt=OtptTxtEdt->textCursor().selectedText(); //Get the selected text.

    QUrl MbAUrl(SltdTxt); //Maybe it's a url.

    QDesktopServices::openUrl(MbAUrl); //Open the url.

    return;
} //void AptF::OpenUrl() //!<The open url action is triggered,then open the url.

/*!
 * \brief AptF::KateEdit Use kate to edit the spec file.
 */
void AptF::KateEdit()
{
    QStringList KtPrms; //The parameter list.
    KtPrms << ui->SpeclineEdit->text(); //Add the spec file path.

    QProcess::startDetached("kate",KtPrms); //Start kate process.

    return;
} //void AptF::KateEdit() //!<Use kate to edit the spec file.

/*!
 * \brief AptF::HomebrewSearch Use homebrew to search for a package.
 */
void AptF::HomebrewSearch()
{
    qDebug() << __FILE__ << __LINE__ << __func__ << tr("package name:") << ui->PkgNmlineEdit_3->currentText(); //Debug.

    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "search" ; //Add the command to search for package.

    AptPrms << ui->PkgNmlineEdit_3->currentText(); //加入package name。


    AptPrc.start(BrewCmd,AptPrms); //启动进程。

    return;
} //void AptF::HomebrewSearch() //!<Use homebrew to search for a package.

/*!
 * \brief AptF::ReportError  Error ocurred,then report error.Report the error of the process.
 * \param AptErr Error code.
 */
void AptF::ReportError(QProcess::ProcessError AptErr)
{
    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Error:") << AptErr; //Report the error.

    switch (AptErr)  //根据不同的错误码做出响应。
    {
    case QProcess::FailedToStart: //启动失败。
        OtptTxtEdt->appendPlainText (tr("The process failed to start.")); //报告错误。
        EnableButtons (0); //重新启用按钮。

        break; //跳出。

    default: //默认情况。
        break; //跳出。
    } //switch (AptErr)  //根据不同的错误码做出响应。

    return;
} //void AptF::ReportError(QProcess::ProcessError AptErr) //!<Error ocurred,then report error.

/*!
 * \brief AptF::RarPasswordCompress Rar compress with password.
 */
void AptF::RarPasswordCompress()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=PasswordCompress; //当前的工作内容是，带密码压缩文件。

    QString Pswd=ui->PswdlineEdit_7->currentText(); //Get the password.
    QString RarFlNm=ui->RarNmlineEdit_6->currentText(); //Get the rar file name.

    QStringList AptPrms; //命令行参数。
    AptPrms << "a" ; //Add the command to compress files.
    AptPrms << QString("-p%1").arg(Pswd); //Add the password parameter.
    AptPrms << "-ep1"; //Do not contain path in the rar file.
    AptPrms << RarFlNm ; //Add the rar file name.
    AptPrms << ui->FlNmlineEdit_5->currentText(); //加入file name。

    AptPrc.start(RarCmd,AptPrms); //启动进程。

    return;
} //void AptF::RarPasswordCompress() //!<Rar compress with password.

/*!
 * \brief AptF::BrewInstall Install package using brew.
 */
void AptF::BrewInstall()
{
    ui->HbrtabWidget_4->setCurrentWidget(ui->Instltab_14); //Switch to homebrew install tab.

    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "install" ; //Add the command to install package.

    AptPrms << ui->PkgNmlineEdit_5->currentText(); //加入package name。

    if (ui->UnvscheckBox->isChecked()) //use the universal option.
    {
        AptPrms << "--universal"; //Add the universal option.
    } //if (ui->UnvscheckBox->isChecked()) //use the universal option.


    AptPrc.start(BrewCmd,AptPrms); //启动进程。


    return;
} //void AptF::BrewInstall() //!<Install package using brew.

/*!
 * \brief AptF::BrewRemove Homebrew remove package.
 */
void AptF::BrewRemove()
{
    qDebug() << __FILE__ << __LINE__ << __func__ << tr("package name:") << ui->PkgNmlineEdit_3->currentText(); //Debug.

    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "rm" ; //Add the command to install package.

    AptPrms << ui->PkgNmlineEdit_6->currentText(); //加入package name。


    AptPrc.start(BrewCmd,AptPrms); //启动进程。


    return;
} //void AptF::BrewRemove() //!<Homebrew remove package.

/*!
 * \brief AptF::Uuidgen Uuidgen.Generate uuid.
 */
void AptF::Uuidgen()
{
    FilterOutput=false; //不过滤输出内容。

    AptPrc.start(UuidgenCmd); //启动进程。


    return;
} //void AptF::Uuidgen() //!<Uuidgen.Generate uuid.

/*!
 * \brief AptF::CopyText The copy text action is triggered,then copy text.
 */
void AptF::CopyText()
{
    QClipboard * ClpBd=QApplication::clipboard(); //Get the clipboard.

    ClpBd->setText(OtptTxtEdt->textCursor().selectedText()); //Set the clipboard.

    return;
} //void AptF::CopyText() //!<The copy text action is triggered,then copy text.

/*!
 * \brief AptF::ChangeBrewFocus Homebrew change ui focus.
 */
void AptF::ChangeBrewFocus(int CrtTb)
{
    switch (CrtTb) //Change focus according to tab index.
    {
    case 1: //Install
        ui->PkgNmlineEdit_5->setFocus(); //Focus the install package line edit.

        break;
    default:
        break;
    } //switch (CrtTb) //Change focus according to tab index.

    return;
} //void AptF::ChangeBrewFocus() //!<Homebrew change ui focus.

/*!
 * \brief AptF::BrewLink Homebrew link package.
 */
void AptF::BrewLink()
{

    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "link" ; //Add the command to link package.

    AptPrms << ui->PkgNmlineEdit_7->currentText(); //加入package name。


    AptPrc.start(BrewCmd,AptPrms); //启动进程。


    return;
} //void AptF::BrewLink() //!<Homebrew link package.

/*
 * \brief AptF::JumpToError Jump to the last error.
 */
void AptF::JumpToError()
{
    QStringList ErrCddt; //Candidate error key words.

    ErrCddt << "error :" << "Error" << "error:" << tr("Found installed(but not packaged)files:"); //Add the key words.

    foreach(QString CrtErrStr,ErrCddt) //Try to match.
    {
        if (OtptTxtEdt->find(CrtErrStr,QTextDocument::FindBackward)) //Find backward.
        {
            break; //Found ,then break.
        } //if (OtptTxtEdt->find("error :",QTextDocument::FindBackward)) //Find backward.

    } //foreach(CrtErrStr,ErrCddt) //Try to match.



    return;
} //void AptF::JumpToError() //!<Jump to the last error.

/*!
 * \brief AptF::OpenCpp The open cpp action is triggered,then open the cpp file.
 */
void AptF::OpenCpp()
{
    QString SltdTxt=OtptTxtEdt->textCursor().selectedText(); //Get the selected text.

    QUrl MbAUrl(SltdTxt); //Maybe it's a url.

    QDesktopServices::openUrl(MbAUrl); //Open the url.


    return;
} //void AptF::OpenCpp() //!<The open cpp action is triggered,then open the cpp file.

/*!
 * \brief AptF::AptCacheSearch Search available packages by apt-cache.
 */
void AptF::AptCacheSearch()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "search" ; //Add the command to search package.

    AptPrms << ui->PkgNmlineEdit_8->currentText(); //加入package name。


    AptPrc.start(AptCacheCmd,AptPrms); //启动进程。

    return;
} //void AptF::AptCacheSearch() //!<Search available packages by apt-cache.

/*!
 * \brief AptF::RpmValidate Rpm validate the package.
 */
void AptF::RpmValidate()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=RpmValidateWorkType; //Work type is rpm validate.

    QStringList AptPrms; //命令行参数。

    if (ui->PkgNmlineEdit_9Vldt->text().isEmpty()) //No package specified.
    {
        AptPrms << "-Va" ; //Add the command to validate package.

    } //if (ui->PkgNmlineEdit_9Vldt->text().isEmpty()) //No package specified.
    else //Specified package name.
    {
        AptPrms << "-V" ; //Add the command to validate package.
        AptPrms << ui->PkgNmlineEdit_9Vldt->text(); //加入package name。

    } //else //Specified package name.

    AptPrc.start(RpmCmd,AptPrms); //启动进程。
} //void AptF::RpmValidate() //!<Rpm validate the package.

/*!
 * \brief AptF::gemUninstall 卸载红宝石。
 */
void AptF::gemUninstall()
{
    ui->FunctabWidget_2->setCurrentWidget(ui->Gemtab); //切换到gem标签页。

    FilterOutput=false; //不过滤输出内容。
    doingWorkType=GemsUninstall; //当前的工作类型是，gem卸载。


    //获取软件包名字：
    QString Pkg2Ist=ui->gemLocalRpmlineEdit_3->text(); //获取软件包名字。

    QStringList Pkg2IstLst=Pkg2Ist.split(" "); //分割字符串。

    Pkg2IstLst.removeAll(""); //删除空白的字符串实例。

    //安装：
    QStringList AptPrms; //命令行参数。
    AptPrms << "uninstall" ; //加入命令，安装。

    AptPrms << Pkg2IstLst; //加入软件包名字。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("AptPrms:") << AptPrms; //Debug.

    AptPrc.start(GemCmd,AptPrms); //启动进程。
} //void AptF::gemUninstall()

/*!
 * \brief AptF::GemInstall 使用gem来安装软件包。
 */
void AptF::GemInstall()
{
    ui->FunctabWidget_2->setCurrentWidget(ui->Gemtab); //切换到gem标签页。

    FilterOutput=false; //不过滤输出内容。
    doingWorkType=GemsInstall; //当前的工作类型是，gem安装。


    //获取软件包名字：
    QString Pkg2Ist=ui->PkgNmlineEdit_9->currentText().trimmed(); //获取软件包名字。

    QStringList Pkg2IstLst=Pkg2Ist.split(" "); //分割字符串。

    Pkg2IstLst.removeAll(""); //删除空白的字符串实例。

    //安装：
    QStringList AptPrms; //命令行参数。
    AptPrms << "install" ; //加入命令，安装。

    if (ui->useProxycheckBox->isChecked ()) //使用代理。
    {
        AptPrms << "--http-proxy" << QString("http://127.0.0.1:%1").arg (httpProxyPort); //加入代理选项。
    } //if (ui->useProxycheckBox->isChecked ()) //使用代理。

    QString packageVersion=ui->versionlineEdit->text(); //获取版本号字符串。

    AptPrms << Pkg2IstLst; //加入软件包名字。

    AptPrms << "--version" << packageVersion; //加入版本号。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("AptPrms:") << AptPrms; //Debug.

    AptPrc.start(GemCmd,AptPrms); //启动进程。

    return;
} //void AptF::GemInstall() //!<使用gem来安装软件包。

/*!
 * \brief AptF::konsoleLaunch 启动konsole.
 */
void AptF::konsoleLaunch()
{

    FilterOutput=false; //不过滤输出内容。

    AptPrc.start(konsoleCmd); //启动进程。

    return;
} //void AptF::konsoleLaunch()

/*!
 * \brief AptF::catMergeFiles 使用cat来merge file.
 */
void AptF::catMergeFiles()
{

    FilterOutput=false; //不过滤输出内容。



    QStringList AptPrms; //命令行参数。

    QStringList Pkg2IstLst=ui->catFileNamePkgNmlineEdit_13->currentText().trimmed().split("\n"); //分割字符串。




    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Pkg2IstLst:") << Pkg2IstLst; //Debug.


    AptPrms << Pkg2IstLst; //加入软件包名字。


    QString prefix=findSplittedFileName(Pkg2IstLst); //The prefix name.

    CatPrc.setStandardOutputFile (prefix); //Set the standard output file .

    CatPrc.start(catCmd,AptPrms); //启动进程。


    return;
} //void AptF::catMergeFiles()

/*!
 * \brief AptF::findSplittedFileName Find the prefix name by part file name.
 * \param Pkg2IstLst The splitted part file name list.
 * \return The recovered initial file name.
 */
QString AptF::findSplittedFileName(QStringList Pkg2IstLst)
{
    QString result="";

    QString singlePath=Pkg2IstLst[0]; //Get the single path.

    int partDotIndex=singlePath.lastIndexOf (".part."); //Get the last index of .part.

    if (partDotIndex<=0) //Not found , or , is the first part.
    {
        result=singlePath+".catMerged"; //Append "catMerged"
    } //if (partDotIndex<=0) //Not found , or , is the first part.
    else //found.
    {
        result=singlePath.left (partDotIndex); //Get the left part.
    } //else //found.



    return result;
} //QString AptF::findSplittedFileName(QStringList Pkg2IstLst)

/*!
 * \brief AptF::splitFile 使用split来split file.
 */
void AptF::splitFile()
{

    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。

    QString sizeString=""; //The size string.

    sizeString.setNum (ui->splitSizespinBox->value ()); //Add the value number.

    QString sizeUnitString=sizeUnitToString(ui->splitSizeUnitcomboBox->currentIndex ()); //Convert unit to string.

#ifndef Q_OS_MACOS

    sizeString+=sizeUnitString; //Add the unit string.
#endif

    AptPrms << "-b" << sizeString; //加入parameter,size。

#ifndef Q_OS_MACOS

    AptPrms << "--verbose"; //verbose output.
#endif

    QString Pkg2IstLst=ui->fileNamePkgNmlineEdit_12->currentText().trimmed(); //Get the file name。



    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Pkg2IstLst:") << Pkg2IstLst; //Debug.


    AptPrms << Pkg2IstLst; //加入软件包名字。


    QString prefix=Pkg2IstLst; //The prefix name.
    prefix.append (".part."); //Append the part name.

    AptPrms << prefix; //Add the prefix parameter.


    AptPrc.start(splitCmd,AptPrms); //启动进程。


    return;
} //void AptF::splitFile()

/*!
 * \brief AptF::sizeUnitToString Convert unit to string.
 * \param uisplitSizeUnitcomboBoxcurrentIndex The index of the current unit.
 * \return The unit string.
 */
QString AptF::sizeUnitToString(int uisplitSizeUnitcomboBoxcurrentIndex)
{
    QString result=""; //Result;

    QMap<int,QString> unitStringMap=
    {
        {0,""},
        {1,"K"},
        {2,"M"},
        {3,"G"},
        {4,"T"},
    }; //Initialize the map.

    result=unitStringMap[uisplitSizeUnitcomboBoxcurrentIndex]; //Get the result.

    return result;
} //QString AptF::sizeUnitToString(int uisplitSizeUnitcomboBoxcurrentIndex)

/*!
 * \brief AptF::yumProvides 使用yum来查询哪个包提供了某个文件.
 */
void AptF::yumProvides()
{

    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "provides"; //加入查询软件包的命令。

    QStringList Pkg2IstLst=ui->PkgNmlineEdit_11->currentText().trimmed().split("\n"); //分割字符串。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Pkg2IstLst:") << Pkg2IstLst; //Debug.


    AptPrms << Pkg2IstLst; //加入软件包名字。


    AptPrc.start(yumCmd,AptPrms); //启动进程。

    return;

} //void AptF::yumProvides()

/*!
 * \brief AptF::yumLocalInstall yum local install.
 */
void AptF::yumLocalInstall()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。
    AptPrms << "localinstall"; //加入安装软件包的命令。

    QStringList Pkg2IstLst=ui->PkgNmlineEdit_10->currentText().trimmed().split("\n"); //分割字符串。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Pkg2IstLst:") << Pkg2IstLst; //Debug.


    AptPrms << Pkg2IstLst; //加入软件包名字。


    AptPrc.start(yumCmd,AptPrms); //启动进程。

    return;
} //void AptF::yumLocalInstall() //!<yum local install.

/*!
 * \brief AptF::srpmRebuild rebuild by src.rpm.
 */
void AptF::srpmRebuild()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。

    if (ui->i686checkBox->isChecked()) //build for i686.
    {
        AptPrms << "--target=i686-gnu-linux"; //target is i686.
    } //if (ui->i686checkBox->isChecked()) //build for i686.


    AptPrms << "--rebuild"; //加入build软件包的命令。

    AptPrms << ui->srpmSpeclineEdit_2->text(); //加入src.rpm文件路径。


    AptPrc.start(RpmBuildCmd,AptPrms); //启动进程。


    return;
} //void AptF::srpmRebuild() //!<rebuild by src.rpm.

/*!
 * \brief AptF::mountDevice 挂载设备文件。
 */
void AptF::mountDevice()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。



    AptPrms << ui->FlNmlineEdit_7->currentText (); //加入设备文件名。

    AptPrms << ui->targetDirNmlineEdit_8->currentText (); //加入挂载点路径。


    AptPrc.start(mountCmd,AptPrms); //启动进程。

    return;
} //void AptF::mountDevice() //!<挂载设备文件。

/*!
 * \brief AptF::installEventFilters 安装各个部件的事件过滤器。
 */
void AptF::installEventFilters()
{
    ui->RpmlineEdit->installEventFilter(this); //安装事件过滤器。
    ui->SpeclineEdit->installEventFilter(this); //安装事件过滤器。
    ui->FilelineEdit_2->installEventFilter(this); //Instal an event filter.
    OtptTxtEdt->installEventFilter(this); //Install an event filter.
    qApp->installEventFilter(this); //Install an event filter to filter the mouse up event.
    ui->FlNmlineEdit_5->lineEdit()->installEventFilter(this); //Install event filter.
    ui->FlNmlineEdit_7->lineEdit ()->installEventFilter (this); //安装事件过滤器。被挂载的设备文件名输入框。
    ui->adbFileNamelineEdit->installEventFilter (this); //安装事件过滤器。APK文件名。
    ui->gemLocalRpmlineEdit_2->installEventFilter (this); //安装事件过滤器。gem文件名。
    ui->fileNamePkgNmlineEdit_12->lineEdit ()->installEventFilter (this); //Install event filter. Split file name.
    ui->catFileNamePkgNmlineEdit_13->lineEdit ()->installEventFilter (this); //Install event filter. Cat file name.

    return;
} //void AptF::installEventFilters() //!<安装各个部件的事件过滤器。

/*!
 * \brief AptF::startService 启动服务。
 */
void AptF::startService()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=ServiceStart; //当前的工作内容是，启动服务。

    QStringList AptPrms; //命令行参数。



    AptPrms << ui->serviceNamelineEdit->text (); //加入服务名。

    AptPrms << "start"; //加入命令，启动服务。


    AptPrc.start(serviceCmd,AptPrms); //启动进程。

    return;
} //void AptF::startService() //!<启动服务。

/*!
 * \brief AptF::restartService 重新启动服务。
 */
void AptF::restartService()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。



    AptPrms << ui->serviceNamelineEdit->text (); //加入服务名。

    AptPrms << "restart"; //加入命令，重新启动服务。


    AptPrc.start(serviceCmd,AptPrms); //启动进程。

    return;
} //void AptF::restartService() //!<重新启动服务。

/*!
 * \brief AptF::checkServiceStatus 检查服务的状态。
 */
void AptF::checkServiceStatus()
{
    FilterOutput=false; //不过滤输出内容。

    QStringList AptPrms; //命令行参数。



    AptPrms << ui->serviceNamelineEdit->text (); //加入服务名。

    AptPrms << "status"; //加入命令，检查服务的状态。


    AptPrc.start(serviceCmd,AptPrms); //启动进程。

    return;
} //void AptF::checkServiceStatus() //!<检查服务的状态。

/*!
 * \brief AptF::adbDevices 列出adb设备列表。
 */
void AptF::adbDevices()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=ApkInstall; //正在安装APK。

    QStringList AptPrms; //命令行参数。

    AptPrms << "devices"; //加入命令，列出设备。

    AptPrc.start(adbCmd,AptPrms); //启动进程。
} //void AptF::adbDevices()

/*!
 * \brief AptF::adbInstall adb安装apk.
 */
void AptF::adbInstall()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=ApkInstall; //正在安装APK。

    QStringList AptPrms; //命令行参数。
    
    if (ui->deviceNamelineEdit->text().isEmpty()) //未指定设备。
    {
        
    } //if (ui->deviceNamelineEdit->text().isEmpty()) //未指定设备。
    else //指定设备。
    {
        AptPrms << "-s" << ui->deviceNamelineEdit->text(); //指定设备。
    } //else //指定设备。

    AptPrms << "install" << "-r"; //加入命令，安装APK。
    
    if (ui->allowTestOnlyApkcheckBox->isChecked()) //允许安装测试应用。
    {
        AptPrms << "-t"; //加入命令，允许安装仅用于测试目的的APK。
        
    } //if (ui->allowTestOnlyApkcheckBox->isChecked()) //允许安装测试应用。

    AptPrms << ui->adbFileNamelineEdit->text (); //加入文件名。

    AptPrc.start(adbCmd,AptPrms); //启动进程。
} //void AptF::adbInstall() //!<adb安装apk.

/*!
 * \brief AptF::killProcess 杀死进程。
 */
void AptF::killProcess()
{
    AptPrc.kill (); //杀死进程。

    return;
} //void AptF::killProcess() //!<杀死进程。

/*!
 * \brief AptF::ttsSayFinished 命令成功退出，则语音报告。
 * \param extCode 命令的退出码。
 */
void AptF::noticeFinished(int extCode)
{
    QString noticeText=""; //要报告的文字内容。

    switch(doingWorkType) //根据当前进行的工作的类型，发出不同的语音报告。
    {
    case RpmInstall: //rpm安装。
        if (extCode==0) //正常退出。
        {
            noticeText=tr("Rpm install finished.");  //报告，RPM包安装完毕。
        } //if (extCode==0) //正常退出。
        else //安装失败。
        {
            noticeText=tr("Rpm install failed."); //报告，RPM包安装失败。
        } //else //安装失败。
        break; //跳出。

    case GemsInstall: //gems安装。
        if (extCode==0) //正常退出。
        {
            noticeText=tr("Gem install finished."); //报告，gems包安装完毕。
        } //if (extCode==0) //正常退出。
        else //安装失败。
        {
            noticeText=tr("Gem install failed."); //报告，gems包安装失败。
        } //else //安装失败。
        break; //跳出。

    case ServiceStart: //服务启动。
        noticeText=tr("Service start finished."); //报告服务启动完毕。
        break; //跳出。

    case PasswordCompress: //带密码压缩文件。
        noticeText=tr("Password compress finished."); //报告，带密码压缩文件完毕。
        break; //跳出。

    case ApkInstall: //APK安装完毕。
        if (extCode==0) //正常退出。
        {
            noticeText=tr("Apk install finished."); //报告，APK安装完毕。
        } //if (extCode==0) //正常退出。
        else //安装失败。
        {
            noticeText=tr("Apk install failed."); //报告，APK安装失败。
        } //else //安装失败。

        break; //跳出。

    case RpmBuild: //RPM打包完毕。
        if (extCode==0) //正常退出。
        {
            noticeText=tr("Rpm package finished."); //报告，RPM打包完毕。
        } //if (extCode==0) //正常退出。
        else //出错。
        {
            noticeText=tr("Rpm package failed."); //报告，RPM打包失败。
        } //else //出错。

        break; //跳出。

    case RpmQuery: //RPM查询完毕。
        noticeText=tr("Rpm query finished."); //报告，RPM查询完毕。

        break; //跳出。

    default: //默认情况。
        break; //跳出。
    } //switch(doingWorkType) //根据当前进行的工作的类型，发出不同的语音报告。

    ttsSay(noticeText); //语音报告。
    toastNotice(noticeText); //吐息报告。


    return;
} //void AptF::ttsSayFinished(int extCode) //!<命令成功退出，则语音报告。

/*!
 * \brief SBrowserWindow::ttsSayLoadFinished 通过TTS报告载入完成。
 */
void AptF::ttsSay(QString string2Say)
{
    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Goting to call tts server."); //Debug.

    ttsClient.saySentence(string2Say); //说话，载入完毕。

    return;
} //void SBrowserWindow::ttsSayLoadFinished() //!<通过TTS报告载入完成。

/*!
   * \brief SBrowserWindow::showSettingDialog 显示设置对话框。
   */
void AptF::showSettingDialog()
{
    stngDlg.show(); //显示对话框。


    return;
} //void SBrowserWindow::showSettingDialog() //!<显示设置对话框。

/*!
 * \brief SBrowserWindow::loadSetting 载入设置信息。
 */
void AptF::loadSetting()
{
    QSettings settings; //设置信息。s

    settings.beginGroup("AptF"); //主程序。


    QString ttsServerAddress=settings.value("ttsServerAddress",QVariant("192.168.2.113")).toString (); //获取tts服务器的地址.

    ttsClient.setServerAddress(ttsServerAddress); //设置服务器地址。
    stngDlg.setTtsServerAddress(ttsServerAddress); //设置TTS服务器地址。

    quint16 ttsServerPort=settings.value("ttsServerPort",QVariant(11245)).toUInt (); //获取tts服务器的端口号。

    ttsClient.setServerPort(ttsServerPort); //设置服务器端口号。
    stngDlg.setTtsServerPort(ttsServerPort); //设置TTS服务器的端口号。

    socksProxyPort=settings.value("socksProxyPort",QVariant(9090)).toUInt (); //获取tts服务器的端口号。

    stngDlg.setSocksProxyPort(socksProxyPort); //设置TTS服务器的端口号。

    httpProxyPort=settings.value("httpProxyPort",QVariant(8118)).toUInt (); //获取tts服务器的端口号。

    stngDlg.setHttpProxyPort(httpProxyPort); //设置TTS服务器的端口号。


    httpProxyAddEnvironmentVariable=settings.value("httpProxyAddEnvironmentVariable",QVariant(false)).toBool(); //获取tts服务器的端口号。

    stngDlg.setHttpProxyAddEnvironmentVariable(httpProxyAddEnvironmentVariable); //设置TTS服务器的端口号。


    settings.endGroup(); //关闭分组。
} //void SBrowserWindow::loadSetting() //!<载入设置信息。

/*!
 * \brief SBrowserWindow::saveSetting 设置对话框确认关闭，则保存设置信息。
 */
void AptF::saveSetting()
{
    QSettings settings; //设置信息储存对象。

    settings.beginGroup("AptF"); //开始分组。

    QVariant ttsServerAddress=QVariant(stngDlg.getTtsServerAddress()); //获取TTS服务器的地址。

    settings.setValue("ttsServerAddress",ttsServerAddress); //tts服务器的地址.

    QVariant ttsServerPort=QVariant(stngDlg.getTtsServerPort()); //获取TTS服务器的端口号。

    settings.setValue("ttsServerPort",ttsServerPort); //tts server's port.

    ttsClient.setServerAddress (stngDlg.getTtsServerAddress ()); //设置TTS服务器的地址。
    ttsClient.setServerPort (stngDlg.getTtsServerPort ()); //设置TTS服务器的端口号。


    socksProxyPort=stngDlg.getSocksProxyPort(); //获取socks5代理的端口号。

    settings.setValue ("socksProxyPort",socksProxyPort); //设置socks5代理的端口号。


    httpProxyPort=stngDlg.getHttpProxyPort(); //获取http代理的端口号。
    settings.setValue ("httpProxyPort",httpProxyPort); //设置http代理的端口号。

    httpProxyAddEnvironmentVariable=stngDlg.getHttpProxyAddEnvironmentVariable();
    settings.setValue("httpProxyAddEnvironmentVariable",httpProxyAddEnvironmentVariable);

    settings.endGroup(); //关闭分组。

    settings.sync(); //同步。
} //void SBrowserWindow::saveSetting() //!<设置对话框确认关闭，则保存设置信息。

/*!
 * \brief AptF::quit 退出。
 */
void AptF::quit()
{
    if (hasRunningProcess()) //还有子进程未退出。
    {
        if (QMessageBox::warning (this,tr("Confirm quiting"),tr("Some operation is executing , are you sure you want to quit?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) //确认要退出。
        {
            QApplication::quit (); //退出。
        } //if (QMessageBox::warning (this,tr("Confirm quiting"),tr("Some operation is executing , are you sure you want to quit?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) //确认要退出。
    } //if (hasRunningProcess()) //还有子进程未退出。
    else //没有正在运行的子进程。
    {
        QApplication::quit (); //退出。
    } //else //没有正在运行的子进程。


    return;
} //void AptF::quit() //!<退出。

/*!
 * \brief AptF::showTray 显示托盘图标。
 */
void AptF::showTray()
{
    QAction * QuitAct=new QAction(this); //退出按钮。
    QuitAct->setText (tr("Quit")); //设置文字。

    connect (QuitAct,&QAction::triggered,this,&AptF::quit); //按退出按钮，则退出。
    connect (&TrayIcn,&QSystemTrayIcon::activated,this,&AptF::AsesShow); //点击左键时，显示主窗口。

    TrayMn.addAction (QuitAct); //退出按钮。


    TrayIcn.setContextMenu (&TrayMn); //设置右键菜单。
    TrayIcn.setIcon (QIcon(":/Images/Icon.png")); //设置托盘图标的图片。
    TrayIcn.show(); //显示托盘图标。


    return;
} //void AptF::showTray()

/*!
 * \brief XyRunner::AsesShow 点击左键时，显示主窗口。
 * \param ActRsn 激活原因。
 */
void AptF::AsesShow(QSystemTrayIcon::ActivationReason ActRsn)
{
    if (ActRsn==QSystemTrayIcon::Trigger) //左键。
    {
        show(); //显示。
    } //if (ActRsn==QSystemTrayIcon::Trigger) //左键。
} //void XyRunner::AsesShow(QSystemTrayIcon::ActivationReason ActRsn) //!<点击左键时，显示主窗口。

/*!
 * \brief AptF::hasRunningProcess 是否还有子进程未退出。
 * \return 是否还有子进程未退出。
 */
bool AptF::hasRunningProcess()
{
    bool result=false; //结果。

    if ((GrepPrc.state ()==QProcess::Running) || (AptPrc.state ()==QProcess::Running)) //有子进程正在运行。
    {
        result=true; //结果为真。
    } //if ((GrepPrc.state ()==QProcess::Running) || (AptPrc.state ()==QProcess::Running)) //有子进程正在运行。

    return result;
} //bool AptF::hasRunningProcess()

/*!
 * @brief 停止服务。
 */
void AptF::stopService()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=ServiceStop; //当前的工作内容是，停止服务。

    QStringList AptPrms; //命令行参数。



    AptPrms << ui->serviceNamelineEdit->text (); //加入服务名。

    AptPrms << "stop"; //加入命令，停止服务。


    AptPrc.start(serviceCmd,AptPrms); //启动进程。
} //void AptF::stopService()

/*!
 * \brief AptF::enableService 启用服务，即，使该服务开机自动启动。
 */
void AptF::enableService()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=ServiceEnable; //当前的工作内容是，启用服务。

    QStringList AptPrms; //命令行参数。



    AptPrms << "enable"; //加入命令，启用服务。


    AptPrms << ui->serviceNamelineEdit->text (); //加入服务名。


    AptPrc.start(systemctlCmd,AptPrms); //启动进程。
} //void AptF::enableService()

/*!
 * \brief AptF::disableService 禁用服务。
 */
void AptF::disableService()
{
    FilterOutput=false; //不过滤输出内容。
    doingWorkType=ServiceDisable; //当前的工作内容是，启用服务。

    QStringList AptPrms; //命令行参数。



    AptPrms << "disable"; //加入命令，启用服务。


    AptPrms << ui->serviceNamelineEdit->text (); //加入服务名。


    AptPrc.start(systemctlCmd,AptPrms); //启动进程。
    
} //void AptF::disableService()

/*!
 * \brief AptF::gemContents 列出某个gem的内容。
 */
void AptF::gemContents()
{
    FilterOutput=false; //不过滤输出内容。

    //获取软件包名字：
    QString Pkg2Ist=ui->gemNamelineEdit->text().trimmed(); //获取软件包名字。

    QStringList Pkg2IstLst=Pkg2Ist.split(" "); //分割字符串。

    Pkg2IstLst.removeAll(""); //删除空白的字符串实例。

    //安装：
    QStringList AptPrms; //命令行参数。
    AptPrms << "contents" << Pkg2IstLst; //加入软件包名字。
    AptPrc.start(GemCmd,AptPrms); //启动进程。
} //void AptF::gemContents()

/*!
 * \brief AptF::gemLocalInstall gem本地安装。
 */
void AptF::gemLocalInstall()
{
    FilterOutput=false; //不过滤输出内容。

    //获取软件包名字：
    QString Pkg2Ist=ui->gemLocalRpmlineEdit_2->text().trimmed(); //获取软件包名字。

    QStringList Pkg2IstLst=Pkg2Ist.split(" "); //分割字符串。

    Pkg2IstLst.removeAll(""); //删除空白的字符串实例。

    //安装：
    QStringList AptPrms; //命令行参数。
    AptPrms << "install" << "--local" << Pkg2IstLst; //加入软件包名字。
    AptPrc.start(GemCmd,AptPrms); //启动进程。

    return;
} //void AptF::gemLocalInstall()

/*!
 * \brief AptF::toastNotice 吐息报告。
 * \param noticeText 要吐息报告的文字内容。
 */
void AptF::toastNotice(QString noticeText)
{
    QWidget * context = this;
    QString text = noticeText;

#ifndef Q_OS_MACOS

    Toast::DurationLength duration = Toast::LENGTH_LONG; //长时间显示。
    Toast::makeText(context, text, duration)->show();

#endif

    return;
} //void AptF::toastNotice(QString noticeText)

/*!
 * \brief AptF::updateProcessId 更新进程编号。
 */
void AptF::updateProcessId()
{
    quint16 processId=AptPrc.processId (); //获取进程编号。

    processIdLabel->setText (tr("Process id:%1").arg (processId)); //显示进程编号。

    return;
} //void AptF::updateProcessId()

/*!
 * \brief AptF::resetProcessId 重置进程编号。
 * \param extCode 退出码。
 */
void AptF::resetProcessId(int extCode)
{
    Q_UNUSED(extCode); //不使用勒个参数。

    processIdLabel->setText (tr("Process id:%1").arg (tr("unknown"))); //重置进程编号。

    return;
} //void AptF::resetProcessId(int extCode)

/*!
 * \brief AptF::updateWindowTitle 更新窗口标题。
 */
void AptF::updateWindowTitle()
{
    int currentIndex=ui->FunctabWidget_2->currentIndex (); //获取大功能标签页部件的当前下标。

    QString functionTitle=ui->FunctabWidget_2->tabText (currentIndex); //获取功能标题。

    QString subFunctionTitle=getSubFunctionTitle(currentIndex); //获取子功能标题。

    setWindowTitle (tr("AptF")+tr("/")+functionTitle+tr("/")+subFunctionTitle); //更新标题。

    return;
} //void AptF::updateWindowTitle()

/*!
 * \brief AptF::getSubFunctionTitle 获取子功能标题。
 * \param currentFunctionIndex 大功能标签页下标。
 * \return 子功能标题。
 */
QString AptF::getSubFunctionTitle(int currentFunctionIndex)
{
    QString result=""; //结果。

    if (functionIndexTabWidgetMap.contains (currentFunctionIndex)) //对应的下标界面有一个子功能标签页。
    {
        QTabWidget * currentFunctionTabWidget=functionIndexTabWidgetMap[currentFunctionIndex]; //获取当前的大功能标签页部件。

        int currentSubFunctionIndex=currentFunctionTabWidget->currentIndex (); //获取当前子功能标签页下标。

        result= currentFunctionTabWidget->tabText (currentSubFunctionIndex); //获取标题。

    } //if (functionIndexTabWidgetMap.contains (currentFunctionIndex)) //对应的下标界面有一个子功能标签页。


    return result;
} //QString AptF::getSubFunctionTitle(int currentFunctionIndex)

/*!
 * \brief AptF::initializeFunctionIndexFunctionTabWidgetMap 初始化大功能标签页下标与大功能标签页部件之间的映射关系。
 */
void AptF::initializeFunctionIndexFunctionTabWidgetMap()
{
    functionIndexTabWidgetMap=
    {
        {0,ui->ApttabWidget},
        {1,ui->RpmtabWidget_3},
        {2,ui->rpmbuildtabWidget},
        {3,ui->HbrtabWidget_4},
        {4,ui->finktabWidget_5},
        {5,ui->rartabWidget_6},
        {7,ui->gemApttabWidget_2},
        {8,ui->yumApttabWidget_3},
    }; //初始化。

    return;
} //void AptF::initializeFunctionIndexFunctionTabWidgetMap()
