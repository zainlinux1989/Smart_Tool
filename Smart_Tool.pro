#-------------------------------------------------
#
# Project created by QtCreator 2016-07-13T17:24:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Smart_Tool
TEMPLATE = app


SOURCES += main.cpp\
        appmainwindow.cpp \
    file_verifier.cpp \
    find_duplicate_files.cpp \
    myqfileinfo.cpp

HEADERS  += appmainwindow.h \
    file_verifier.h \
    find_duplicate_files.h \
    myqfileinfo.h

FORMS    += appmainwindow.ui

RESOURCES += \
    resource.qrc
