TEMPLATE = app
CONFIG += gui
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp Shader.cpp

LIBS += -lglut -lGL -lGLU -lfreeimage -lGLEW

HEADERS += \
    Shader.h

DISTFILES += \
    ../build-ShaderTest-Desktop_Qt_5_3_GCC_32bit-Debug/colortopbottom.frag \
    ../build-ShaderTest-Desktop_Qt_5_3_GCC_32bit-Debug/colortopbottom.vert \
    ../build-ShaderTest-Desktop_Qt_5_3_GCC_32bit-Debug/grayscale.frag \
    ../build-ShaderTest-Desktop_Qt_5_3_GCC_32bit-Debug/grayscale.vert \
    ../build-ShaderTest-Desktop_Qt_5_3_GCC_32bit-Debug/color.frag \
    ../build-ShaderTest-Desktop_Qt_5_3_GCC_32bit-Debug/glow.frag \
    ../build-ShaderTest-Desktop_Qt_5_3_GCC_32bit-Debug/color.vert \
    ../build-ShaderTest-Desktop_Qt_5_3_GCC_32bit-Debug/glow.vert
