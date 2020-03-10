QT       += core gui sql charts xlsx

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
# QXlsx code for Application Qt project
TRANSLATIONS += chinese.ts \ chinese.qm
SOURCES += \
    addaccountform.cpp \
    counsellorform.cpp \
    counsellorinfoform.cpp \
    courseinfoform.cpp \
    loginform.cpp \
    main.cpp \
    mainwindow.cpp \
    querystudentform.cpp \
    querystudentgradeform.cpp \
    studentform.cpp \
    studentinfoform.cpp \
    teacherform.cpp \
    teacherinfoform.cpp

HEADERS += \
    addaccountform.h \
    counsellorform.h \
    counsellorinfoform.h \
    courseinfoform.h \
    loginform.h \
    mainwindow.h \
    querystudentform.h \
    querystudentgradeform.h \
    studentform.h \
    studentinfoform.h \
    teacherform.h \
    teacherinfoform.h

FORMS += \
    addaccountform.ui \
    counsellorform.ui \
    counsellorinfoform.ui \
    courseinfoform.ui \
    loginform.ui \
    mainwindow.ui \
    querystudentform.ui \
    querystudentgradeform.ui \
    studentform.ui \
    studentinfoform.ui \
    teacherform.ui \
    teacherinfoform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
