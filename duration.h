#ifndef DURATION_H
#define DURATION_H

#include <QObject>
#include <QVariant>

class Duration
{
    unsigned long long m_msecs;

public:
    Duration()
    {
        m_msecs = 0;
    }

    QString toString()
    {
        QString str;
        unsigned long long hours = m_msecs / 1000 / 60 / 60;
        if (hours < 10) str += "0";
        str += QVariant(hours).toString();
        str += ":";
        unsigned long long minutes = m_msecs / 1000 / 60 - 60 * hours;
        if (minutes < 10) str += "0";
        str += QVariant(minutes).toString();
        str += ":";
        unsigned long long seconds = m_msecs / 1000 - 60 * 60 * hours - 60 * minutes;
        if (seconds < 10) str += "0";
        str += QVariant(seconds).toString();
        return str;
    }

    void reset()
    {
        m_msecs = 0;
    }

    void operator+=(Duration& right)
    {
        m_msecs += right.m_msecs;
    }
    void add(unsigned long long delta)
    {
        m_msecs += delta;
    }

    unsigned long long int msecs()
    {
        return m_msecs;
    }
};

#endif // DURATION_H

