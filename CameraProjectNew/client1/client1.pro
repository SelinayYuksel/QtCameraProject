QT += core network gui

CONFIG += c++17 cmdline
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# OpenCV Kütüphaneleri
INCLUDEPATH += /usr/include/opencv4
LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_videoio
SOURCES += \
        client.cpp \
        counterworker.cpp \
        main.cpp \
        timeworker.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    client.h \
    counterworker.h \
    timeworker.h
