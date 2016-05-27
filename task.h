#ifndef TASK_H
#define TASK_H

#include <QObject>

#include "event.h"
#include "duration.h"

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = 0);

signals:

public slots:

private:
    QList<Task*> mChildren;
    QMap<QString, Event*> mEvents;

public:
    QString mName;
    QString mUID;
    Duration mDuration;
};

#endif // TASK_H
