cache

TEMPLATE = app

TARGET = firstedit

QT += core gui widgets

CONFIG += release

unix {
	target.path = /usr/local/bin
	INSTALLS += target
}

INCLUDEPATH += GeneratedFiles
MOC_DIR = GeneratedFiles
OBJECTS_DIR = Objects
UI_DIR = GeneratedFiles
RCC_DIR = GeneratedFiles

HEADERS += firstedit.h

SOURCES += main.cpp firstedit.cpp

FORMS += firstedit.ui
