QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aptf
TEMPLATE = app

# 版本号定义
VERSION = 1.1.0
DEFINES += APP_VERSION=\"\\\"$$VERSION\\\"\"

SOURCES += main.cpp\
        AptF.cpp \
    PlainTextEditMouseRelease.cpp \
    AptfTtsClient.cpp \
    AptfSettingDialog.cpp

HEADERS  += AptF.h \
    PreCompile.h \
    PlainTextEditMouseRelease.h \
    AptfTtsClient.h \
    AptfSettingDialog.h

FORMS    += AptF.ui \
    AptfSettingDialog.ui

CONFIG+=precompile_header
PRECOMPILED_HEADER+=PreCompile.h

TRANSLATIONS+=AptF.ts



RESOURCES += \
    Images.qrc

QMAKE_CXXFLAGS+= -std=c++17 -DVERSION=\"\\\"$$VERSION\\\"\"

macx {
QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+

CONFIG +=c++11
} #macx {


OTHER_FILES += \
    CMakeLists.txt \
    aptf.desktop \
    Images/aptf.png

target.path=/usr/bin #目标文件的安装路径。
INSTALLS+=target #安装目标文件。

desktopFile.path=/usr/share/applications
desktopFile.files=aptf.desktop

INSTALLS+=desktopFile

iconFile.path=/usr/share/icons
iconFile.files=Images/aptf.png
INSTALLS+=iconFile


QT+=network #加入网络模块。
unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/ -ltoast

QT += core5compat