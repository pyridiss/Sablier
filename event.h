#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QDateTime>

#include <libical/icaltypes.h>

class Task;

class Event : public QObject
{
    Q_OBJECT
public:
    explicit Event(QObject *parent = 0);

signals:

public slots:

public:
    Task* pParent;
    QString mName;
    QString mUID;
    QDateTime mStartTime;
    QDateTime mEndTime;

public:
    std::string icalSummary();
    std::string icalUid();
    std::string icalRelatedTo();
    icaltimetype icalDtStart();
    icaltimetype icalDtEnd();
};

#endif // EVENT_H
