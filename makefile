all: main.cpp
	g++ main.cpp -o shot_em_up.o -std=c++11 \
	-lallegro -lallegro_audio -lallegro_video -lallegro_acodec -lallegro_primitives \
	-lallegro_ttf -lallegro_dialog -lallegro_font -lallegro_image \
	-lGL -lGLU -lglut\
	