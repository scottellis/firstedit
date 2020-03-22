#ifndef FIRSTEDIT_H
#define FIRSTEDIT_H

#include <QtWidgets/QMainWindow>
#include <qfontcombobox.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qshortcut.h>

#include "ui_firstedit.h"

class FirstEdit : public QMainWindow
{
    Q_OBJECT

public:
    FirstEdit(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);

protected slots:
    void zoomIn();
    void zoomOut();

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
    QShortcut *m_quit;
    QShortcut *m_zoomIn;
    QShortcut *m_zoomOut;
};

#endif // FIRSTEDIT_H
