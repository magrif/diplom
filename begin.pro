#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T09:12:43
#
#-------------------------------------------------

QT       += core gui axcontainer

RC_FILE = res.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = begin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CZKFPEngX.cpp

HEADERS  += mainwindow.h \
    CZKFPEngX.h

FORMS    += mainwindow.ui


win32: LIBS += -L$$PWD/../../../../"Program Files (x86)"/Veridis/"Biometric SDK"/lib/x86/ -lVrBio

INCLUDEPATH += $$PWD/../../../../"Program Files (x86)"/Veridis/"Biometric SDK"/inc
DEPENDPATH += $$PWD/../../../../"Program Files (x86)"/Veridis/"Biometric SDK"/inc
