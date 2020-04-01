#ifndef FIRSTEDIT_H
#define FIRSTEDIT_H

#include <QtWidgets/QMainWindow>
#include <qfontcombobox.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qprogressbar.h>

#include "ui_firstedit.h"
#include "battery.h"

class FirstEdit : public QMainWindow
{
    Q_OBJECT

public:
    FirstEdit(QWidget *parent = 0);
    ~FirstEdit();

public slots:
    void zoomIn();
    void zoomOut();
    void batteryStatusChange(QString);
    void batteryLevelChange(int);
    void startBatteryThread();

protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    void layoutWindow();
    void restoreWindowState();
    void saveWindowState();
    void loadCache();
    void saveCache();
    void setFontSize(int size);
    QString settingsFile();
    QString cacheFile();

    Ui::FirstEditClass ui;

    int m_fontSize;
    QTextEdit *m_edit;
    QPushButton *m_quitBtn;
    QLabel *m_capsLockStatus;
    QLabel *m_batteryStatus;
    QProgressBar *m_batteryLevel;
    BatteryThread *m_batteryThread;
};

#endif // FIRSTEDIT_H
