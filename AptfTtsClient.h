#ifndef APTF_TTSCLIENT_H
#define APTF_TTSCLIENT_H

#include <QObject>

#include "PreCompile.h" //QTcpSocket

/*!
 * \brief The SbTtsClient class TTS客户端类。
 */
class AptfTtsClient : public QObject
{
    Q_OBJECT
public:
    void saySentence(QString trLoadfinished); //!<说话。
    explicit AptfTtsClient(QObject *parent = 0);
    void setServerAddress(QString ttsServerAddress2St); //!<设置服务器地址。
    void setServerPort(quint16 ttsServerPort2St); //!<设置服务器端口号。

private:
    void startConnectTtsServer(); //!<开始连接至TTS服务器。
    QByteArray protclBtArr; //!<要发送给TTS服务器的字节数组内容。
    void connectSignals(); //!<连接信号。
    QTcpSocket tcpClient; //!<TCP客户端对象。
    QString mTtsServerAddress; //!<TTS服务器的地址。
    quint16 mTtsServerPort; //!<TTS服务器的端口号。

private slots:
    void sendTtsCommand(); //!<发送之前待发送的命令。

signals:

public slots:

}; //class SbTtsClient : public QObject

#endif // SBTTSCLIENT_H
