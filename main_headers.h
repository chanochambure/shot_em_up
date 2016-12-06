#ifndef INCLUDED_MAIN_HEADERS_H
#define INCLUDED_MAIN_HEADERS_H

#include <iostream>
#include <list>

#include "LexRisLogic/Allegro5/Allegro.h"
#include "LexRisLogic/MathStructures/Point.h"
#include "LexRisLogic/Math.h"
#include "LexRisLogic/Convert.h"
#include "LexRisLogic/Time.h"
#include <allegro5/allegro_opengl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define SIZE_X              800.0
#define SIZE_Y              600.0
#define SIZE_Z              800.0
#define INITIAL_X           0
#define INITIAL_Y           0
#define INITIAL_Z           400

#define MIN_X               -400
#define MAX_X               400
#define MIN_Y               -300
#define MAX_Y               300

#define MIN_ENEMY_X         -900
#define MIN_ENEMY_Y         -700
#define MIN_ENEMY_Z         -450
#define MAX_ENEMY_X         900
#define MAX_ENEMY_Y         700
#define MAX_ENEMY_Z         450

#define MOVE_UP             "U"
#define MOVE_DOWN           "D"
#define MOVE_LEFT           "L"
#define MOVE_RIGHT          "R"
#define PAUSE               "P"
#define GRID                "G"

#define TIME_TO_SHOT        0.6
#define SHOT_SIZE           10

#define COLLISION_TOLERANCE 0.8
#define COLLISION_TOLERANCE_Z 0.4

#include "Class/meteors.h"
#include "Class/player.h"
#include "Class/Block.h"

#endif //INCLUDED_MAIN_HEADERS_H
