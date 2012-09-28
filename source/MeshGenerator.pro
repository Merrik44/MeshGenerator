#-------------------------------------------------
#
# Project created by QtCreator 2012-09-05T02:56:18
#
#-------------------------------------------------

QT       += core gui

TARGET = MeshGenerator
TEMPLATE = app


SOURCES += main.cpp\
        glwidget.cpp \
    vmath.cpp \
    mesh.cpp \
    debug.cpp \
    mainwindow.cpp \
    lstskeleton.cpp \
    meshgenerator.cpp \
    objexporter.cpp \
    jointgenerator.cpp \
    surfacesubdivision.cpp


HEADERS  += mainwindow.h \
glwidget.h \
    vmath.h \
    mesh.h \
    helper.h \
    debug.h \
    lstskeleton.h \
    branchnode.h \
    meshgenerator.h \
    objexporter.h \
    jointgenerator.h \
    surfacesubdivision.h

QT += opengl
FORMS    += mainwindow.ui
