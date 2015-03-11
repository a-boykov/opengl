TEMPLATE = app
CONFIG += gui
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp Shader.cpp

LIBS += -lglut -lGL -lGLU -lfreeimage -lGLEW

HEADERS += \
    Shader.h

DISTFILES += \
    glsl/colortopbottom.frag \
    glsl/colortopbottom.vert \
    glsl/grayscale.frag \
    glsl/grayscale.vert \
    glsl/color.frag \
    glsl/glow.frag \
    glsl/color.vert \
    glsl/glow.vert \
    data/mask001.png \
    data/mask002.png \
    data/mask003.png \
    data/mask004.png \
    data/mask005.png \
    data/mask006.png \
    data/pick_01.png \
    data/pick_02.png \
    data/screen.png \
    data/texture1.png \
    data/texture2.png \
    data/texture2old.png \
    data/texture3.png
