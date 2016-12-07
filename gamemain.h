#ifndef INCLUDED_GAMEMAIN_H
#define INCLUDED_GAMEMAIN_H

#include "main_headers.h"

double dt=0;
double ttime=0;
double ttime_base=0;

void openGL_configuration()
{
    glEnable(GL_NORMALIZE);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat position[] = { 0,SIZE_Z/2,0, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,SIZE_X/SIZE_Y,0.1,SIZE_Z*2);
    glTranslatef(0,0,-3*SIZE_Z/5);
//    glRotatef(45,1,0,0);
//    glRotatef(45,0,1,0);
}

void start_game(LL_AL5::Display* display,LL_AL5::Input* input)
{
    int score=0;
    int max_score=0;

    LL_AL5::KeyControl key_control;
    key_control.add_key(MOVE_UP,ALLEGRO_KEY_W);
    key_control.add_key(MOVE_DOWN,ALLEGRO_KEY_S);
    key_control.add_key(MOVE_LEFT,ALLEGRO_KEY_A);
    key_control.add_key(MOVE_RIGHT,ALLEGRO_KEY_D);
    key_control.add_key(PAUSE,ALLEGRO_KEY_ENTER);
    key_control.add_key(GRID,ALLEGRO_KEY_G);
    input->set_key_control(&key_control);
    LL::random_generate_new_seed();
    LL::Chronometer timer;
    timer.play();
    openGL_configuration();
    Player player;
    player.set_pos(INITIAL_X,INITIAL_Y,INITIAL_Z);
    meteorsEngine engine;
    Block block(SIZE_X,SIZE_Y,SIZE_Z);
    bool draw_grid=false;
    bool pause=false;
    EnhancerEngine enhancer;
    LL_AL5::Audio stage;
    stage.set_path("stage.ogg");
    stage.load();
    stage.set_playmode(ALLEGRO_PLAYMODE_LOOP);
    stage.play();
    LL_AL5::Audio end_game;
    end_game.set_path("end.ogg");
    end_game.load();
    end_game.set_playmode(ALLEGRO_PLAYMODE_LOOP);
    while(!input->get_display_status())
    {
        if(input->get_event())
        {
            //Delta Time
            ttime=timer.get_time();
            dt=ttime-ttime_base;
            ttime_base=ttime;
            if((*input)[PAUSE])
            {
                pause=!pause;
                (*input)[PAUSE]=false;
            }
            if(pause)
                continue;
            player.set_up((*input)[MOVE_UP]);
            player.set_down((*input)[MOVE_DOWN]);
            player.set_left((*input)[MOVE_LEFT]);
            player.set_right((*input)[MOVE_RIGHT]);
            player.move(dt);
            engine.move(dt);
            enhancer.move();
            enhancer.enhancer_collision(&player);
            int to_upgrade=engine.meteor_collision(&player);
            if(to_upgrade==-1)
            {
                stage.stop();
                end_game.play();
                std::cout<<"Perdiste :v"<<std::endl;
                LL_AL5::sleep(3.0);
                input->clear_events();
                input->clear_key_status();
                player.clear();
                player.set_pos(INITIAL_X,INITIAL_Y,INITIAL_Z);
                engine.clear();
                enhancer.clear();
                if(max_score<score)
                    max_score=score;
                score=0;
                end_game.stop();
                stage.play();
            }
            else
                score+=to_upgrade;
            if((*input)[GRID])
            {
                draw_grid=!draw_grid;
                (*input)[GRID]=false;
            }
            if(input->get_timer_event())
            {
                system("clear");
                std::cout<<"MAX Score: "<<max_score<<std::endl;
                std::cout<<"Score: "<<score<<std::endl;
                std::cout<<"Pos: "<<player.get_pos()<<std::endl;
                player.message();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                display->clear_to_color(LL_AL5::Color(0,0,0));
                if(draw_grid)
                {
                    glDisable(GL_LIGHTING);
                        block.draw();
                    glEnable(GL_LIGHTING);
                }
                engine.draw();
                player.draw();
                enhancer.draw();
                display->refresh();
            }
        }
    }
    stage.stop();
}

#endif // INCLUDED_GAMEMAIN_H

