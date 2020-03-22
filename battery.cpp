#include <qfile.h>
#include <qtextstream.h>

#include "battery.h"

BatteryThread::BatteryThread(int updateInterval, QObject *parent)
    : QThread(parent)
{
    if (updateInterval < 1)
        m_updateInterval = 1;
    else
        m_updateInterval = updateInterval;

    m_batteryLevel = 0;
    m_batteryStatus = "Unknown";
    m_abort = false;
    m_sysPath = "/sys/class/power_supply/BAT0";
}

BatteryThread::~BatteryThread()
{
    m_mutex.lock();
    m_abort = true;
    m_mutex.unlock();

    wait();
}

QString BatteryThread::batteryStatus()
{
    QMutexLocker lock(&m_mutex);
    return m_batteryStatus;
}

int BatteryThread::batteryLevel()
{
    QMutexLocker lock(&m_mutex);
    return m_batteryLevel;
}

QString BatteryThread::readStatus()
{
    QFile file(m_sysPath + "/status");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "Error";

    QTextStream in(&file);
    QString txt = in.readLine();
    file.close();

    return txt;
}

int BatteryThread::readCapacity()
{
    QFile file(m_sysPath + "/capacity");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    QTextStream in(&file);
    QString txt = in.readLine();
    file.close();

    bool ok;
    int capacity = txt.toInt(&ok);

    if (ok)
        return capacity;
    else
        return -1;
}

void BatteryThread::run()
{
    while (!m_abort) {
        QString status = readStatus();

        if (status == m_batteryStatus) {
            m_batteryStatus = status;
            emit batteryStatusChange(m_batteryStatus);
        }

        int capacity = readCapacity();

        if (capacity != m_batteryLevel) {
            m_batteryLevel = capacity;
            emit batteryLevelChange(m_batteryLevel);
        }

        sleep(m_updateInterval);
    }
}
