#include <qdir.h>
#include <qsettings.h>
#include <qboxlayout.h>
#include <qformlayout.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qevent.h>

#include "firstedit.h"

#define MIN_FONTSIZE 8
#define MAX_FONTSIZE 120

FirstEdit::FirstEdit(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    layoutWindow();

    connect(m_quitBtn, SIGNAL(pressed()), SLOT(close()));

    restoreWindowState();

    loadCache();
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

    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->addWidget(m_edit);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(m_quitBtn);
    hLayout->addStretch();
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

