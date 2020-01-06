CONFIG += c++11
TEMPLATE = app
DEST = jehova

SOURCES += src/main.cpp \
    src/canvas.cpp \
    src/network.cpp \
    src/avahiservice.cpp

INCLUDEPATH += /usr/include/SDL2
			+= /usr/include/

LIBS += -lSDL2
LIBS +=	-lSDL2_image
LIBS += -lSDL2_ttf

HEADERS += \
    src/canvas.h \
    src/network.h \
    src/avahiservice.h
