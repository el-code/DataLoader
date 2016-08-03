QT += core network
QT -= gui

TARGET = DataLoader
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    loader.cpp

HEADERS += \
    loader.hpp

