#ifndef BATTERY_H
#define BATTERY_H

#include <qthread.h>

#define DEFAULT_UPDATE_INTERVAL 5

class BatteryThread : public QThread
{
    Q_OBJECT

public:
    BatteryThread(int updateInterval = DEFAULT_UPDATE_INTERVAL, QObject *parent = nullptr);
    ~BatteryThread();

    QString batteryStatus();
    int batteryLevel();

signals:
    void batteryStatusChange(QString);
    void batteryLevelChange(int);

protected:
    void run() override;

private:
    QString readStatus();
    int readCapacity();

    QString m_sysPath;
    bool m_abort;
    int m_updateInterval;
    QString m_batteryStatus;
    int m_batteryLevel;
};

#endif // BATTERY_H
