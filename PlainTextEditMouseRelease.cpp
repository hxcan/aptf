#include "PlainTextEditMouseRelease.h"

/*!
 * \brief PlainTextEditMouseRelease::PlainTextEditMouseRelease 构造函数。
 * \param parent 亲代对象。
 */
PlainTextEditMouseRelease::PlainTextEditMouseRelease(QWidget *parent) :QPlainTextEdit(parent)
{
    setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff); //水平方向隐藏滚动条。
} //PlainTextEditMouseRelease::PlainTextEditMouseRelease(QWidget *parent) :QPlainTextEdit(parent)

/*!
 * \brief PlainTextEditMouseRelease::mouseReleaseEvent The mouse release event.
 * \param event The event.
 */
void PlainTextEditMouseRelease::mouseReleaseEvent ( QMouseEvent * event )
{
    emit MouseReleased(); //emit the signal , mouse released.

    QPlainTextEdit::mouseReleaseEvent(event); //Process it.

    return;
} //void PlainTextEditMouseRelease::mouseReleaseEvent ( QMouseEvent * event ) //!<The mouse release event.
