#ifndef INCLUDED_PLAYER_H
#define INCLUDED_PLAYER_H

#include "../main_headers.h"

class Player
{
    private:
        LL_AL5::Image _V_sprite_player;
        LL_AL5::Image _V_sprite_shot;
        GLuint _V_texture_player=0;
        GLuint _V_texture_shot=0;
        GLfloat _V_max_speed=300.0;
        GLfloat _V_pos_x=0.0;
        GLfloat _V_pos_y=0.0;
        int _V_size_x=20.0;
        int _V_size_y=40.0;
        bool _V_up_status=false;
        bool _V_down_status=false;
        bool _V_left_status=false;
        bool _V_right_status=false;
        LL::Chronometer _V_timer;
        typedef std::pair<LL_MathStructure::Point<2>,float> Direction_Shot;
        typedef std::pair<float,Direction_Shot> Weapon_Shot;
        std::list<Weapon_Shot> shots;
        float _V_time_to_shot;
        int _V_shot_speed=400.0;
        int _V_shot_max_distance=700.0;
    public:
        Player()
        {
            _V_timer.play();
            _V_time_to_shot=TIME_TO_SHOT;
            _V_sprite_player.set_path("nave.png");
            _V_sprite_player.load();
            _V_texture_player = al_get_opengl_texture(_V_sprite_player);
            _V_sprite_shot.set_path("fire.png");
            _V_sprite_shot.load();
            _V_texture_shot = al_get_opengl_texture(_V_sprite_shot);
        }
        void set_pos(GLfloat new_x,GLfloat new_y)
        {
            _V_pos_x=new_x;
            _V_pos_y=new_y;
        }
        void set_up(bool up_status)
        {
            _V_up_status=up_status;
        }
        void set_down(bool down_status)
        {
            _V_down_status=down_status;
        }
        void set_left(bool left_status)
        {
            _V_left_status=left_status;
        }
        void set_right(bool right_status)
        {
            _V_right_status=right_status;
        }
        void move(double dt=1)
        {
            int move_x=_V_right_status-_V_left_status;
            int move_y=_V_down_status-_V_up_status;
            set_pos(_V_pos_x+move_x*_V_max_speed*dt,
                    _V_pos_y+move_y*_V_max_speed*dt);
            if(_V_pos_x<MIN_X)
                _V_pos_x=MIN_X;
            else if(_V_pos_x>MAX_X)
                _V_pos_x=MAX_X;
            if(_V_pos_y<MIN_Y)
                _V_pos_y=MIN_Y;
            else if(_V_pos_y>MAX_Y)
                _V_pos_y=MAX_Y;
            for(std::list<Weapon_Shot>::iterator iter=shots.begin();iter!=shots.end();)
            {
                if((*iter).first<_V_shot_max_distance)
                {
                    (*iter).first+=(_V_shot_speed*dt);
                    ++iter;
                }
                else
                    iter=shots.erase(iter);
            }
            shot();
        }
        void shot()
        {
            if(_V_timer.get_time()>_V_time_to_shot)
            {
                float angle=LL::sexagesimal_to_radian(-90);
                shots.push_back(Weapon_Shot(0,Direction_Shot(LL_MathStructure::create_point(_V_pos_x,_V_pos_y),angle)));
                _V_timer.clear();
            }
        }
        void draw()
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, _V_texture_player);

            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
            glBegin(GL_QUADS);
                glTexCoord2f(1, 1);
                glVertex3f(_V_pos_x-_V_size_x/2,_V_pos_y-_V_size_y/2,0.0);

                glTexCoord2f(0,1);
                glVertex3f(_V_pos_x+_V_size_x/2,_V_pos_y-_V_size_y/2,0.0);

                glTexCoord2f(0, 0);
                glVertex3f(_V_pos_x+_V_size_x/2,_V_pos_y+_V_size_y/2,0.0);

                glTexCoord2f(1, 0);
                glVertex3f(_V_pos_x-_V_size_x/2,_V_pos_y+_V_size_y/2,0.0);
            glEnd();
            glDisable(GL_TEXTURE_2D);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, _V_texture_shot);

            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
            glBegin(GL_QUADS);
            for(std::list<Weapon_Shot>::iterator iter=shots.begin();iter!=shots.end();++iter)
            {
                float pos_x=((*iter).second).first[0]+((*iter).first*cos(((*iter).second).second));
                float pos_y=((*iter).second).first[1]+((*iter).first*sin(((*iter).second).second));
                glTexCoord2f(1, 1);
                glVertex3f(pos_x-SHOT_SIZE/2.0, pos_y-SHOT_SIZE/2.0, 0.0);

                glTexCoord2f(0, 1);
                glVertex3f(pos_x-SHOT_SIZE/2.0, pos_y+SHOT_SIZE/2.0, 0.0);

                glTexCoord2f(0, 0);
                glVertex3f(pos_x+SHOT_SIZE/2.0, pos_y+SHOT_SIZE/2.0, 0.0);

                glTexCoord2f(1, 0);
                glVertex3f(pos_x+SHOT_SIZE/2.0, pos_y-SHOT_SIZE/2.0, 0.0);
            }
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        void clear()
        {
            _V_timer.clear();
            shots.clear();
        }

        int meteor_collision(meteor meteoro)
        {
            for(std::list<Weapon_Shot>::iterator iter=shots.begin();iter!=shots.end();++iter)
            {
                float pos_x=((*iter).second).first[0]+((*iter).first*cos(((*iter).second).second));
                float pos_y=((*iter).second).first[1]+((*iter).first*sin(((*iter).second).second));
                if(LL::segment_collision(meteoro.getPosX()-meteoro.get_size()/2,meteoro.getPosX()+meteoro.get_size()/2,
                                         pos_x-SHOT_SIZE/2,pos_x+SHOT_SIZE/2))
                {
                    if(LL::segment_collision(meteoro.getPosY()-meteoro.get_size()/2,meteoro.getPosY()+meteoro.get_size()/2,
                                             pos_y-SHOT_SIZE/2,pos_y+SHOT_SIZE/2))
                        return 1;
                }
            }
            if(LL::segment_collision(meteoro.getPosX()-meteoro.get_size()/2,meteoro.getPosX()+meteoro.get_size()/2,
                                     _V_pos_x-_V_size_x/2,_V_pos_x+_V_size_x/2))
            {
                if(LL::segment_collision(meteoro.getPosY()-meteoro.get_size()/2,meteoro.getPosY()+meteoro.get_size()/2,
                                         _V_pos_y-_V_size_y/2,_V_pos_y+_V_size_y/2))
                    return 2;
            }
            return 0;
        }
        ~Player()
        {
            glDeleteTextures(1, &_V_texture_player);
            glDeleteTextures(1, &_V_texture_shot);
        }
};

#endif // INCLUDED_PLAYER_H

