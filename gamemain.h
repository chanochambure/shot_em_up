#ifndef INCLUDED_GAMEMAIN_H
#define INCLUDED_GAMEMAIN_H

#include "main_headers.h"

double dt=0;
double ttime=0;
double ttime_base=0;

void openGL_configuration()
{glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
}

void start_game(LL_AL5::Display* display,LL_AL5::Input* input)
{

    LL_AL5::KeyControl key_control;
    key_control.add_key(MOVE_UP,ALLEGRO_KEY_W);
    key_control.add_key(MOVE_DOWN,ALLEGRO_KEY_S);
    key_control.add_key(MOVE_LEFT,ALLEGRO_KEY_A);
    key_control.add_key(MOVE_RIGHT,ALLEGRO_KEY_D);
    key_control.add_key(PAUSE,ALLEGRO_KEY_ENTER);
    input->set_key_control(&key_control);
    LL::random_generate_new_seed();
    LL::Chronometer timer;
    timer.play();
    openGL_configuration();
    Player player;
    player.set_pos(INITIAL_X,INITIAL_Y);
    while(!input->get_display_status())
    {
        //Delta Time
        ttime=timer.get_time();
        dt=ttime-ttime_base;
        ttime_base=ttime;
        player.set_up((*input)[MOVE_UP]);
        player.set_down((*input)[MOVE_DOWN]);
        player.set_left((*input)[MOVE_LEFT]);
        player.set_right((*input)[MOVE_RIGHT]);
        player.move(dt);
        if(input->get_event())
        {
            if(input->left_click())
            {
                player.shot(input->get_mouse_x(),input->get_mouse_y());
            }
            if(input->get_timer_event())
            {
                display->clear_to_color(LL_AL5::Color());
                player.draw();
                display->refresh();
            }
        }
    }
}

#endif // INCLUDED_GAMEMAIN_H

