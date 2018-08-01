#-------------------------------------------------
#
# Project created by QtCreator 2018-06-29T19:04:42
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gpio.cpp \
    uart.cpp \
    adc.cpp \
    pwm.cpp \
    can.cpp \
    consumer_producer.cpp

HEADERS  += mainwindow.h \
    gpio.h \
    common.h \
    uart.h \
    adc.h \
    pwm.h \
    can.h \
    consumer_producer.h

