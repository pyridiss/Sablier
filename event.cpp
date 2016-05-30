#include "event.h"

Event::Event(QObject *parent) : QObject(parent)
{
    mStartTime = QDateTime::currentDateTime();
    mEndTime = QDateTime::currentDateTime();

}

