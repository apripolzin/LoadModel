QT += core gui widgets opengl openglwidgets

CONFIG += c++11

SOURCES += \
    abstractmesh.cpp \
    camera.cpp \
    cone.cpp \
    cube.cpp \
    main.cpp \
    mesh.cpp \
    vertex.cpp \
    widget.cpp \
    obj_loader.cpp

HEADERS += \
    abstractmesh.h \
    camera.h \
    cone.h \
    cube.h \
    mesh.h \
    vertex.h \
    widget.h \
    obj_loader.h

RESOURCES += \
    models.qrc \
    shaders.qrc \
    textures.qrc
