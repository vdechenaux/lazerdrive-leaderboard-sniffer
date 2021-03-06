QT += core websockets sql
QT -= gui

TARGET = lazerdrive-leaderboard-sniffer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    lazerdriveapp.cpp \
    lazerdriveconfiguration.cpp \
    lazerdrivedatabasemanager.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QLazerDriveClient/release/ -lQLazerDriveClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QLazerDriveClient/debug/ -lQLazerDriveClient
else:unix: LIBS += -L$$PWD/QLazerDriveClient/ -lQLazerDriveClient

INCLUDEPATH += $$PWD/QLazerDriveClient
DEPENDPATH += $$PWD/QLazerDriveClient

HEADERS += \
    lazerdriveapp.h \
    lazerdriveconfiguration.h \
    lazerdrivedatabasemanager.h \
    lazerdrivecacheentry.h
