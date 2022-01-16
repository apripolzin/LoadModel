QT += core gui widgets opengl openglwidgets

CONFIG += c++11

SOURCES += \
    camera.cpp \
    cube.cpp \
    main.cpp \
    mesh.cpp \
    vertex.cpp \
    widget.cpp

HEADERS += \
    camera.h \
    cube.h \
    mesh.h \
    vertex.h \
    widget.h

RESOURCES += \
    models.qrc \
    shaders.qrc \
    textures.qrc
