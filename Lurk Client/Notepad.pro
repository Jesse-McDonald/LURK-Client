#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T14:43:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Notepad
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += g++14
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        notepad.cpp \
    connectionwidow.cpp \
    server.cpp \
    LURK.cpp \
    error.cpp \
    verify.cpp \
    characterdisplay.cpp

HEADERS  += notepad.h \
    connectionwidow.h \
    server.h \
    LURK.h \
    error.h \
    verify.h \
    characterdisplay.h

FORMS    += notepad.ui \
    connectionwidow.ui \
    server.ui \
    error.ui \
    verify.ui \
    characterdisplay.ui
