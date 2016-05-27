#include "task.h"

Task::Task(QObject *parent) : QObject(parent)
{
    pParent = NULL;
}

std::string Task::icalSummary()
{
    return mName.toStdString();
}

std::string Task::icalUid()
{
    return mUID.toStdString();
}

std::string Task::icalRelatedTo()
{
    if (pParent == NULL) return "";

    return pParent->icalUid();
}
