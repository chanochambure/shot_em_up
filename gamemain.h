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
    LL_AL5::Font font;
    font.set_path("comic.ttf");
    font.set_size(24);
    font.load_ttf_font();

    LL_AL5::Text text;
    text.set_color(LL_AL5::Color(255,255,255));
    text.set_pos(50,50);
    text.set_font(&font);
    text="Score";

    LL_AL5::Text text_score;
    text_score.set_color(LL_AL5::Color(255,255,255));
    text_score.set_pos(60,70);
    text_score.set_font(&font);
    text_score.set_flag(ALLEGRO_ALIGN_CENTER);
    text_score="0";

    int score=0;

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
    meteorsEngine engine;
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
        engine.move(dt);
        int to_upgrade=engine.meteor_collision(&player);
        if(to_upgrade==-1)
        {
            player.clear();
            player.set_pos(INITIAL_X,INITIAL_Y);
            engine.clear();
            score=0;
        }
        else
            score+=to_upgrade;
        if(input->get_event())
        {
            if(input->left_click())
            {
                player.shot(input->get_mouse_x(),input->get_mouse_y());
            }
            if(input->get_timer_event())
            {
                text_score=LL::to_string(score);
                display->clear_to_color(LL_AL5::Color());
                player.draw();
                engine.draw();
                display->draw(&text);
                display->draw(&text_score);
                display->refresh();
            }
        }
    }
}

#endif // INCLUDED_GAMEMAIN_H

