QT       += core gui
QT       += serialport
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    p18_device.cpp \
    p18_file.cpp \
    p18_plot.cpp \
    p18_serial.cpp

HEADERS += \
    fifo.h \
    mainwindow.h \
    p18_device.h \
    p18_file.h \
    p18_plot.h \
    p18_serial.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
