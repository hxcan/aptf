#ifndef APTF_SETTINGDIALOG_H
#define APTF_SETTINGDIALOG_H

#include <QDialog>


namespace Ui {
class AptfSettingDialog;
}

class AptfSettingDialog : public QDialog
{
    Q_OBJECT

public:
    quint16 getHttpProxyPort(); //!<获取http代理的端口号。
    quint16 getSocksProxyPort(); //!<获取socks5代理的端口号。
    void setHttpProxyPort(quint16 httpProxyPort); //!<设置http代理服务器的端口号。
    void setSocksProxyPort(quint16 socksProxyPort); //!<设置socks5代理服务器的端口号。
    void setTtsServerPort(quint16 ttsServerPort); //!<设置TTS服务器的端口号。
    void setTtsServerAddress(QString ttsServerAddress); //!<设置TTS服务器地址。
    quint16 getTtsServerPort(); //!<获取TTS服务器的端口号。
    QString getTtsServerAddress(); //!<获取TTS服务器的地址。
    explicit AptfSettingDialog(QWidget *parent = 0); //!<构造函数。
    ~AptfSettingDialog(); //!<析构函数。
    
    bool getHttpProxyAddEnvironmentVariable() const;
    void setHttpProxyAddEnvironmentVariable(bool value);

protected:
    void changeEvent(QEvent *e); //!<变更事件。
    
private:
    Ui::AptfSettingDialog *ui;
};

#endif // SETTINGDIALOG_H
