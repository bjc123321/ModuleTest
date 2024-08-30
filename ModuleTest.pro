#-------------------------------------------------
#
# Project created by QtCreator 2024-08-22T15:47:03
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModuleTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialportmanager.cpp \
    modbusprotocolparser.cpp

HEADERS  += mainwindow.h \
    serialportmanager.h \
    modbusprotocolparser.h

FORMS    += mainwindow.ui
