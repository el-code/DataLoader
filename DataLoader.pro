QT += core network
QT -= gui

QMAKE_CXXFLAGS += -std=c++14

TARGET = DataLoader
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    loader.cpp \
    parser.cpp

HEADERS += \
    loader.hpp \
    parser.hpp

