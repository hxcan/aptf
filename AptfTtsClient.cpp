#include "AptfTtsClient.h" //AptfTtsClient

/*!
 * \brief SbTtsClient::SbTtsClient 构造函数。
 * \param parent 亲代对象。
 */
AptfTtsClient::AptfTtsClient(QObject *parent) :QObject(parent)
{
    connectSignals(); //连接信号。

} //SbTtsClient::SbTtsClient(QObject *parent) :QObject(parent)

/*!
 * \brief SbTtsClient::setServerAddress 设置服务器地址。
 * \param ttsServerAddress2St 要设置的服务器地址。
 */
void AptfTtsClient::setServerAddress(QString ttsServerAddress2St)
{
    mTtsServerAddress=ttsServerAddress2St; //记录。

    return;
} //void SbTtsClient::setServerAddress(QString ttsServerAddress2St) //!<设置服务器地址。

/*!
 * \brief SbTtsClient::setServerPort 设置服务器端口号。
 * \param ttsServerPort2St 要设置的服务器端口号。
 */
void AptfTtsClient::setServerPort(quint16 ttsServerPort2St)
{
    mTtsServerPort=ttsServerPort2St; //记录。

    return;
} //void SbTtsClient::setServerPort(quint16 ttsServerPort2St) //!<设置服务器端口号。

/*!
 * \brief SbTtsClient::startConnectTtsServer 开始连接至TTS服务器。
 */
void AptfTtsClient::startConnectTtsServer()
{
    tcpClient.abort (); //断开之前的连接。

    tcpClient.connectToHost(mTtsServerAddress, mTtsServerPort); //连接服务器。

    return;
} //void SbTtsClient::startConnectTtsServer() //!<开始连接至TTS服务器。

/*!
 * \brief SbTtsClient::saySentence 说话。
 * \param trLoadfinished 要说的话。
 */
void AptfTtsClient::saySentence(QString trLoadfinished)
{
    QString protclString=""; //按照协议构造的字符串。
    //参考的格式：BEGIN|A|16|请36125号徐静蕾到万花筒做检查|END

    protclString="BEGIN|A|16|"+trLoadfinished+"|END\n"; //组装成的发声命令字符串。


    protclBtArr=protclString.toUtf8 (); //转换成UTF8的字节数组。



    startConnectTtsServer (); //开始连接到服务器。连接完毕之后，才会发送文字命令过去。


    return;
} //void SbTtsClient::saySentence(QString trLoadfinished) //!<说话。

/*!
 * \brief SbTtsClient::connectSignals 连接信号。
 */
void AptfTtsClient::connectSignals()
{
    connect (&tcpClient,SIGNAL(connected()),this,SLOT(sendTtsCommand())); //连接完毕，则发送之前待发送的命令。

    return;
} //void SbTtsClient::connectSignals() //!<连接信号。

/*!
 * \brief SbTtsClient::sendTtsCommand 发送之前待发送的命令。
 */
void AptfTtsClient::sendTtsCommand()
{
    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Sending command to tts server.") << tr("ttsClient is connected?:") << tcpClient.state (); //Debug.



    tcpClient.write (protclBtArr); //写入内容。


    tcpClient.close (); //断开连接。

    return;
} //void SbTtsClient::sendTtsCommand() //!<发送之前待发送的命令。
