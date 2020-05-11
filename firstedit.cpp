#include <qdir.h>
#include <qsettings.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qevent.h>
#include <qtimer.h>

#include "firstedit.h"

#define MIN_FONTSIZE 2
#define MAX_FONTSIZE 240

FirstEdit::FirstEdit(QWidget *parent)
    : QMainWindow(parent)
{
    m_batteryThread = nullptr;

    ui.setupUi(this);

    layoutWindow();

    connect(m_quitBtn, SIGNAL(pressed()), SLOT(close()));

    restoreWindowState();

    loadCache();

    QTimer::singleShot(2000, this, SLOT(startBatteryThread()));
}

FirstEdit::~FirstEdit()
{
    if (m_batteryThread)
        delete m_batteryThread;
}

void FirstEdit::startBatteryThread()
{
    m_batteryThread = new BatteryThread(5, this);

    connect(m_batteryThread, SIGNAL(batteryStatusChange(QString)), SLOT(batteryStatusChange(QString)));
    connect(m_batteryThread, SIGNAL(batteryLevelChange(int)), SLOT(batteryLevelChange(int)));

    m_batteryThread->start();
}

void FirstEdit::batteryStatusChange(QString status)
{
    m_batteryStatus->setText(status);
}

void FirstEdit::batteryLevelChange(int level)
{
    m_batteryLevel->setValue(level);
}

void FirstEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_Plus:
            zoomIn();
            break;

        case Qt::Key_Down:
        case Qt::Key_Minus:
            zoomOut();
            break;

        case Qt::Key_Q:
            close();
            break;

        default:
            QWidget::keyPressEvent(event);
            break;
        }
    }
    else {
        QWidget::keyPressEvent(event);
    }
}

void FirstEdit::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_CapsLock) {
        if (m_capsLockStatus->text().length() == 0) {
            m_capsLockStatus->setText("CAPS");
        }
        else {
            m_capsLockStatus->setText("");
        }
    }

    QWidget::keyReleaseEvent(event);
}

void FirstEdit::zoomIn()
{
    if (m_fontSize < MAX_FONTSIZE) {
        m_fontSize += 4;
        setFontSize(m_fontSize);
    }
}

void FirstEdit::zoomOut()
{
    if (m_fontSize > MIN_FONTSIZE) {
        m_fontSize -= 4;
        setFontSize(m_fontSize);
    }
}

void FirstEdit::setFontSize(int size)
{
    QFont newFont(m_edit->font());
    newFont.setPixelSize(size);
    m_edit->setFont(newFont);
}

void FirstEdit::layoutWindow()
{
    m_edit = new QTextEdit;
    m_quitBtn = new QPushButton("&Quit");

    m_batteryStatus = new QLabel("Unknown");
    m_batteryLevel = new QProgressBar();
    m_batteryLevel->setRange(0, 100);
    m_batteryLevel->setValue(0);

    m_capsLockStatus = new QLabel("");
    m_capsLockStatus->setMinimumWidth(60);
    m_capsLockStatus->setFrameShape(QFrame::Panel);
    m_capsLockStatus->setFrameShadow(QFrame::Sunken);
    m_capsLockStatus->setLineWidth(2);

    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->addWidget(m_edit);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_quitBtn);
    hLayout->addStretch();

    hLayout->addWidget(m_capsLockStatus);
    hLayout->addSpacing(20);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(m_batteryStatus, m_batteryLevel);
    hLayout->addLayout(formLayout);

    vLayout->addLayout(hLayout);

    centralWidget()->setLayout(vLayout);
}

void FirstEdit::closeEvent(QCloseEvent *)
{
    saveCache();
    saveWindowState();
}

void FirstEdit::loadCache()
{
    QFile file(cacheFile());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString txt = in.readAll();
    file.close();

    m_edit->setText(txt);
}

void FirstEdit::saveCache()
{
    QFile file(cacheFile());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << m_edit->toPlainText();

    file.close();
}

void FirstEdit::restoreWindowState()
{
    QString iniFile = settingsFile();

    if (iniFile.isEmpty())
    	return;

    QSettings *settings = new QSettings(iniFile, QSettings::IniFormat);

    if (!settings)
    	return;

    settings->beginGroup("Window");
    restoreGeometry(settings->value("Geometry").toByteArray());
    restoreState(settings->value("State").toByteArray());
    settings->endGroup();

    settings->beginGroup("Font");
    m_fontSize = settings->value("Size", 32).toInt();
    settings->endGroup();

    delete settings;

    m_fontSize &= 0xfc;

    if (m_fontSize < MIN_FONTSIZE)
        m_fontSize = MIN_FONTSIZE;
    else if (m_fontSize > MAX_FONTSIZE)
        m_fontSize = MAX_FONTSIZE;

    setFontSize(m_fontSize);
}

void FirstEdit::saveWindowState()
{
    QString iniFile = settingsFile();

    if (iniFile.isEmpty())
    	return;

    QSettings *settings = new QSettings(iniFile, QSettings::IniFormat);

    if (!settings)
    	return;

    settings->beginGroup("Window");
    settings->setValue("Geometry", saveGeometry());
    settings->setValue("State", saveState());
    settings->endGroup();

    settings->beginGroup("Font");
    settings->setValue("Size", m_fontSize);
    settings->endGroup();

    delete settings;
}

QString FirstEdit::settingsFile()
{
    QString home = QDir::homePath();
    return home + "/firstedit.ini";
}

QString FirstEdit::cacheFile()
{
    QString home = QDir::homePath();
    return home + "/firstedit.cache";
}

