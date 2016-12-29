#-------------------------------------------------
#
# Project created by QtCreator 2016-02-21T02:56:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViewPic
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    pictures/pictures.qrc

DISTFILES += \
    pictures/icon.rc
RC_FILE = pictures/icon.rc
