#ifndef INCLUDED_PLAYER_H
#define INCLUDED_PLAYER_H

#include "../main_headers.h"

class Player
{
    private:
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
        }
        void shot(int shot_x,int shot_y)
        {
            if(_V_timer.get_time()>_V_time_to_shot)
            {
                int x=shot_x-_V_pos_x;
                int y=shot_y-_V_pos_y;
                float angle=0;
                if(x or y)
                {
                    angle=acos(x/sqrt(pow(x,2)+pow(y,2)));
                    if(y<0)
                        angle*=-1;
                }
                shots.push_back(Weapon_Shot(0,Direction_Shot(LL_MathStructure::create_point(_V_pos_x,_V_pos_y),angle)));
                _V_timer.clear();
            }
        }
        void draw()
        {
            glColor3d(255,255,255);
            glBegin(GL_TRIANGLES);
                glVertex3f(_V_pos_x,_V_pos_y-_V_size_y/2,0.0);
                glVertex3f(_V_pos_x-_V_size_x/2,_V_pos_y+_V_size_y/2,0.0);
                glVertex3f(_V_pos_x+_V_size_x/2,_V_pos_y+_V_size_y/2,0.0);
            glColor3d(255,0,0);
            glEnd();;
            glBegin(GL_QUADS);
            for(std::list<Weapon_Shot>::iterator iter=shots.begin();iter!=shots.end();++iter)
            {
                glVertex3f(((*iter).second).first[0]+((*iter).first*cos(((*iter).second).second))-SHOT_SIZE/2,
                            ((*iter).second).first[1]+((*iter).first*sin(((*iter).second).second))-SHOT_SIZE/2,
                            0.0);
                glVertex3f(((*iter).second).first[0]+((*iter).first*cos(((*iter).second).second))-SHOT_SIZE/2,
                            ((*iter).second).first[1]+((*iter).first*sin(((*iter).second).second)+SHOT_SIZE/2),
                            0.0);
                glVertex3f(((*iter).second).first[0]+((*iter).first*cos(((*iter).second).second))+SHOT_SIZE/2,
                            ((*iter).second).first[1]+((*iter).first*sin(((*iter).second).second))+SHOT_SIZE/2,
                            0.0);
                glVertex3f(((*iter).second).first[0]+((*iter).first*cos(((*iter).second).second))+SHOT_SIZE/2,
                            ((*iter).second).first[1]+((*iter).first*sin(((*iter).second).second))-SHOT_SIZE/2,
                            0.0);
            }
            glEnd();
        }
};

#endif // INCLUDED_PLAYER_H

