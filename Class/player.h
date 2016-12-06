#ifndef INCLUDED_PLAYER_H
#define INCLUDED_PLAYER_H

#include "../main_headers.h"

class Player
{
    private:
        GLfloat _V_max_speed=300.0;
        LL_MathStructure::Point<3> _V_position;
        GLfloat _V_size;
        bool _V_up_status=false;
        bool _V_down_status=false;
        bool _V_left_status=false;
        bool _V_right_status=false;
        LL::Chronometer _V_timer;
        typedef std::pair<LL_MathStructure::Point<3>,float> Direction_Shot;
        typedef std::pair<float,Direction_Shot> Weapon_Shot;
        std::list<Weapon_Shot> shots;
        float _V_time_to_shot;
        int _V_shot_speed=350.0;
        int _V_shot_max_distance=900.0;
    public:
        Player()
        {
            _V_size=40;
            _V_timer.play();
            _V_time_to_shot=TIME_TO_SHOT;
        }
        void set_pos(GLfloat new_x,GLfloat new_y,GLfloat new_z)
        {
            _V_position[0]=new_x;
            _V_position[1]=new_y;
            _V_position[2]=new_z;
        }
        LL_MathStructure::Point<3> get_pos()
        {
            return _V_position;
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
            int move_y=_V_up_status-_V_down_status;
            set_pos(_V_position[0]+move_x*_V_max_speed*dt,
                    _V_position[1]+move_y*_V_max_speed*dt,
                    _V_position[2]);
            if(_V_position[0]<MIN_X)
                _V_position[0]=MIN_X;
            else if(_V_position[0]>MAX_X)
                _V_position[0]=MAX_X;
            if(_V_position[1]<MIN_Y)
                _V_position[1]=MIN_Y;
            else if(_V_position[1]>MAX_Y)
                _V_position[1]=MAX_Y;
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
                shots.push_back(Weapon_Shot(0,Direction_Shot(LL_MathStructure::create_point(_V_position[0],
                                                                                            _V_position[1],
                                                                                            _V_position[2]),angle)));
                _V_timer.clear();
            }
        }
        void draw()
        {
            GLfloat ambient[] = { 0.2, 0.0, 0.0, 1.0};
            GLfloat diffuse[] = { 0.6, 0.0, 0.0, 1.0 };
            GLfloat	specular[] = { 0.8, 0.0, 0.0, 1.0 };
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            for(std::list<Weapon_Shot>::iterator iter=shots.begin();iter!=shots.end();++iter)
            {
                float pos_x=((*iter).second).first[0]+((*iter).first*cos(((*iter).second).second));
                float pos_y=((*iter).second).first[1];
                float pos_z=((*iter).second).first[2]+((*iter).first*sin(((*iter).second).second));
                glPushMatrix();
                    glTranslatef(pos_x,pos_y,pos_z);
                    glutSolidSphere(SHOT_SIZE,4,4);
                glPopMatrix();
            }
            glPushMatrix();
                GLfloat nave_ambient[] = { 0.0, 0.0, 0.4, 1.0};
                GLfloat nave_diffuse[] = { 0.0, 0.0, 0.6, 1.0 };
                GLfloat	nave_specular[] = { 0.0, 0.0, 0.8, 1.0 };
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, nave_ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, nave_diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, nave_specular);

                glTranslatef(_V_position[0],_V_position[1],_V_position[2]);
                glutSolidSphere(_V_size,6,6);
            glPopMatrix();
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
                float pos_y=((*iter).second).first[1];
                float pos_z=((*iter).second).first[2]+((*iter).first*sin(((*iter).second).second));
                if(LL::segment_collision(meteoro.getPosX()-meteoro.get_size()/2,meteoro.getPosX()+meteoro.get_size()/2,
                                         pos_x-SHOT_SIZE/2,pos_x+SHOT_SIZE/2))
                {
                    if(LL::segment_collision(meteoro.getPosY()-meteoro.get_size()*COLLISION_TOLERANCE/2,
                                             meteoro.getPosY()+meteoro.get_size()*COLLISION_TOLERANCE/2,
                                             pos_y-SHOT_SIZE/2,pos_y+SHOT_SIZE/2))
                    {
                        if(LL::segment_collision(meteoro.getPosZ()-meteoro.get_size()*COLLISION_TOLERANCE/2,
                                             meteoro.getPosZ()+meteoro.get_size()*COLLISION_TOLERANCE/2,
                                             pos_z-SHOT_SIZE/2,pos_z+SHOT_SIZE/2))
                            return 1;
                    }
                }
            }
            if(LL::segment_collision(meteoro.getPosX()-meteoro.get_size()*COLLISION_TOLERANCE/2,
                                     meteoro.getPosX()+meteoro.get_size()*COLLISION_TOLERANCE/2,
                                     _V_position[0]-_V_size/2,_V_position[0]+_V_size/2))
            {
                if(LL::segment_collision(meteoro.getPosY()-meteoro.get_size()*COLLISION_TOLERANCE/2,
                                         meteoro.getPosY()+meteoro.get_size()*COLLISION_TOLERANCE/2,
                                         _V_position[1]-_V_size/2,_V_position[1]+_V_size/2))
                {
                    if(LL::segment_collision(meteoro.getPosZ()-meteoro.get_size()*COLLISION_TOLERANCE_Z/2,
                                             meteoro.getPosZ()+meteoro.get_size()*COLLISION_TOLERANCE_Z/2,
                                             _V_position[2]-_V_size/2,_V_position[2]+_V_size/2))
                        return 2;
                }
            }
            return 0;
        }
        ~Player()
        {
        }
};

#endif // INCLUDED_PLAYER_H

