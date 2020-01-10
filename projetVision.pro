QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gameLogique.cpp \
    main.cpp \
    mainwindow.cpp \
    visionFunction.cpp

HEADERS += \
    gameLogique.h \
    mainwindow.h \
    visionFunction.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += C:\Users\HP\opencv\build\include

LIBS += C:\Users\HP\opencv\opencv-build\bin\libopencv_core320.dll
LIBS += C:\Users\HP\opencv\opencv-build\bin\libopencv_highgui320.dll
LIBS += C:\Users\HP\opencv\opencv-build\bin\libopencv_imgcodecs320.dll
LIBS += C:\Users\HP\opencv\opencv-build\bin\libopencv_imgproc320.dll
LIBS += C:\Users\HP\opencv\opencv-build\bin\libopencv_features2d320.dll
LIBS += C:\Users\HP\opencv\opencv-build\bin\libopencv_calib3d320.dll
LIBS += C:\Users\HP\opencv\opencv-build\bin\libopencv_videoio320.dll
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
