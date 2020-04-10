TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

SOURCES += \
        SleepChronogram.cpp \
        main.cpp

HEADERS += \
    SleepChronogram.h
