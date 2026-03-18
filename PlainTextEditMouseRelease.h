#ifndef PLAINTEXTEDITMOUSERELEASE_H
#define PLAINTEXTEDITMOUSERELEASE_H

#include <QPlainTextEdit>

class PlainTextEditMouseRelease : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEditMouseRelease(QWidget *parent = 0);
    
signals:
    void MouseReleased(); //!<Mouse released.
    
public slots:

protected:
    void mouseReleaseEvent ( QMouseEvent * event ); //!<The mouse release event.
    
};

#endif // PLAINTEXTEDITMOUSERELEASE_H
