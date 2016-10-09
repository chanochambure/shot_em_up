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

#define SIZE_X          400
#define SIZE_Y          700
#define INITIAL_X       200
#define INITIAL_Y       600
#define MIN_X           0
#define MAX_X           400
#define MIN_Y           200
#define MAX_Y           700
#define MIN_ENEMY_X     -50
#define MIN_ENEMY_Y     -50
#define MAX_ENEMY_X     450
#define MAX_ENEMY_Y     750

#define MOVE_UP         "U"
#define MOVE_DOWN       "D"
#define MOVE_LEFT       "L"
#define MOVE_RIGHT      "R"
#define PAUSE           "P"

#define TIME_TO_SHOT    0.8
#define SHOT_SIZE       10

#include "Class/meteors.h"
#include "Class/player.h"

#endif //INCLUDED_MAIN_HEADERS_H
