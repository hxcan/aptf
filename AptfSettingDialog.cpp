#include "PreCompile.h" //QInputDialog

#include "AptfSettingDialog.h" //AptfSettingDialog
#include "ui_AptfSettingDialog.h" //AptfSettingDialog

AptfSettingDialog::AptfSettingDialog(QWidget *parent) :QDialog(parent),ui(new Ui::AptfSettingDialog)
{
    ui->setupUi(this);
}

AptfSettingDialog::~AptfSettingDialog()
{
    delete ui;
}

void AptfSettingDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool AptfSettingDialog::getHttpProxyAddEnvironmentVariable() const
{
    return ui->addEnvironmentVariablecheckBox->isChecked();
}

void AptfSettingDialog::setHttpProxyAddEnvironmentVariable(bool value)
{
    ui->addEnvironmentVariablecheckBox->setChecked(value);
}












/*!
 * \brief StbSettingDialog::getTtsServerAddress 获取TTS服务器的地址。
 * \return TTS服务器的地址。
 */
QString AptfSettingDialog::getTtsServerAddress()
{
    QString result; //结果。

    result=ui->ttsServerAddresslineEdit->text (); //返回输入框内容。

    return result;
} //QString StbSettingDialog::getTtsServerAddress() //!<获取TTS服务器的地址。

/*!
 * \brief StbSettingDialog::getTtsServerPort 获取TTS服务器的端口号。
 * \return TTS服务器的端口号。
 */
quint16 AptfSettingDialog::getTtsServerPort()
{
    quint16 result=0; //结果。

    result=ui->ttsServerPortspinBox->value (); //结果即为旋转框的值。

    return result;
} //quint16 StbSettingDialog::getTtsServerPort() //!<获取TTS服务器的端口号。

/*!
 * \brief StbSettingDialog::setTtsServerAddress 设置TTS服务器地址。
 * \param ttsServerAddress 要设置的TTS服务器地址。
 */
void AptfSettingDialog::setTtsServerAddress(QString ttsServerAddress)
{
    ui->ttsServerAddresslineEdit->setText (ttsServerAddress); //显示。

    return;
} //void StbSettingDialog::setTtsServerAddress(QString ttsServerAddress) //!<设置TTS服务器地址。

/*!
 * \brief StbSettingDialog::setTtsServerPort 设置TTS服务器的端口号。
 * \param ttsServerPort 要设置的TTS服务器端口号。
 */
void AptfSettingDialog::setTtsServerPort(quint16 ttsServerPort)
{
    ui->ttsServerPortspinBox->setValue (ttsServerPort); //显示。

    return;
} //void StbSettingDialog::setTtsServerPort(quint16 ttsServerPort) //!<设置TTS服务器的端口号。

/*!
 * \brief AptfSettingDialog::setSocksProxyPort 设置socks5代理服务器的端口号。
 * \param socksProxyPort 要设置的端口号。
 */
void AptfSettingDialog::setSocksProxyPort(quint16 socksProxyPort)
{
    ui->smtPrxyPrtspinBox->setValue (socksProxyPort); //设置值。

    return;
} //void AptfSettingDialog::setSocksProxyPort(quint16 socksProxyPort)

/*!
 * \brief AptfSettingDialog::setHttpProxyPort 设置http代理服务器的端口号。
 * \param httpProxyPort 要设置的端口号。
 */
void AptfSettingDialog::setHttpProxyPort(quint16 httpProxyPort)
{
    ui->smtPrxyPrtspinBox_2->setValue (httpProxyPort); //设置值。


    return;
} //void AptfSettingDialog::setHttpProxyPort(quint16 httpProxyPort)

/*!
 * \brief AptfSettingDialog::getSocksProxyPort 获取socks5代理的端口号。
 * \return socks5代理的端口号。
 */
quint16 AptfSettingDialog::getSocksProxyPort()
{
    return ui->smtPrxyPrtspinBox->value ();
} //quint16 AptfSettingDialog::getSocksProxyPort()

/*!
 * \brief AptfSettingDialog::getHttpProxyPort 获取http代理的端口号。
 * \return http代理的端口号。
 */
quint16 AptfSettingDialog::getHttpProxyPort()
{
    return ui->smtPrxyPrtspinBox_2->value ();
} //quint16 AptfSettingDialog::getHttpProxyPort()
