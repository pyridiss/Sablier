#include "event.h"
#include "task.h"

Event::Event(QObject *parent) : QObject(parent)
{
    mStartTime = QDateTime::currentDateTime();
    mEndTime = QDateTime::currentDateTime();
}

std::string Event::icalSummary()
{
    return mName.toStdString();
}

std::string Event::icalUid()
{
    return mUID.toStdString();
}

std::string Event::icalRelatedTo()
{
    if (pParent == NULL) return "";

    return pParent->icalUid();
}

icaltimetype Event::icalDtStart()
{
    icaltimetype start;
    start.year   = mStartTime.date().year();
    start.month  = mStartTime.date().month();
    start.day    = mStartTime.date().day();
    start.hour   = mStartTime.time().hour();
    start.minute = mStartTime.time().minute();
    start.second = mStartTime.time().second();

    return start;
}

icaltimetype Event::icalDtEnd()
{
    icaltimetype end;
    end.year   = mEndTime.date().year();
    end.month  = mEndTime.date().month();
    end.day    = mEndTime.date().day();
    end.hour   = mEndTime.time().hour();
    end.minute = mEndTime.time().minute();
    end.second = mEndTime.time().second();

    return end;
}
