QT -= gui core

CONFIG += c++11

TARGET = iMX6_units_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    gpio.cpp \
    uart.cpp

HEADERS += \
    common.h
