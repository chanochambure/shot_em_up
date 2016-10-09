TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp

HEADERS +=  main_boid.h\
			main_headers.h \
			gamemain.h \
			Class/player.h \
			Class/meteors.h

LIBS +=	-lGL\
		-lGLU\
		-lglut

LIBS +=	-lallegro -lallegro_audio -lallegro_video -lallegro_acodec -lallegro_primitives \
		-lallegro_ttf -lallegro_dialog -lallegro_font -lallegro_image

