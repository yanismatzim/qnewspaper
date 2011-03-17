# -------------------------------------------------
# Project created by QtCreator 2009-12-11T16:27:22
# -------------------------------------------------
QT += network \
    xml
TARGET = qNewsPaper
TEMPLATE = app
OTHER_FILES += qnewspaper_fr.ts \
    qnewspaper_en.ts \
    qNewsPaper.rc \
    qNewsPaper.pro \
    qNewsPaper.png \
    qNewsPaper.nsi \
    qNewsPaper.ico \
    Licence.txt \
    Changelog.txt
HEADERS += newspaper.h \
    network.h \
    mainwindow.h
SOURCES += updater.cpp \
    preferences.cpp \
    newspaper.cpp \
    network.cpp \
    makegui.cpp \
    mainwindow.cpp \
    main.cpp
