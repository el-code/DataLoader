QT += core network
QT -= gui

QMAKE_CXXFLAGS += -std=c++14

TARGET = DataLoader
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += /home/artur/boost_1_61_0

LIBS += -L/home/artur/boost_1_61_0/stage/lib -lboost_system

SOURCES += main.cpp \
    loader.cpp \
    parser.cpp

HEADERS += \
    loader.hpp \
    parser.hpp

