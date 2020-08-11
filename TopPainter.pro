
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = TopPainter
#CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    TopPainterWidget.cpp \
    ImageButton.cpp \
    XGlobalHook.cpp \

HEADERS += \
    TopPainterWidget.h \
    ImageButton.h \
    XGlobalHook.h 
