QT  += core gui serialport widgets charts

greaterThan(QT_MAJOR_VERSION, 4) : QT += quickwidgets positioning location

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mywindow.cpp

HEADERS += \
    mywindow.h

FORMS += \
    mywindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc

DISTFILES += \
    icons/Qt-Icon.png \
    icons/satellite-Icon.png