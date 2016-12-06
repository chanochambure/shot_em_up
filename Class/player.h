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
        float _V_bullet_size;
        float _V_time_to_shot;
        int _V_shot_speed=350.0;
        int _V_shot_max_distance=2000.0;
        LL::Chronometer _V_timer_giants_shots;
        bool _V_giants_shots=false;
        LL::Chronometer _V_timer_fast_shot;
        bool _V_fast_shot=false;
        std::list<LL_AL5::Audio*> _V_list_to_destroy;
        LL::Chronometer _V_timer_temp;
    public:
        Player()
        {
            _V_timer_temp.play();
            _V_size=40;
            _V_timer.play();
            _V_time_to_shot=TIME_TO_SHOT;
            _V_bullet_size=SHOT_SIZE;
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
            if(_V_giants_shots)
            {
                if(_V_timer_giants_shots.get_time()>=ENHANCER_GIANT_SHOT_DURATION)
                {
                    _V_giants_shots=false;
                    _V_timer_giants_shots.stop();
                }
            }
            if(_V_fast_shot)
            {
                if(_V_timer_fast_shot.get_time()>=ENHANCER_FAST_SHOT_DURATION)
                {
                    _V_fast_shot=false;
                    _V_timer_fast_shot.stop();
                }
            }
            shot();
        }
        void shot()
        {
            if(_V_timer.get_time()>_V_time_to_shot)
            {
                LL_AL5::Audio* _V_shot_sound=new LL_AL5::Audio();
                _V_shot_sound->set_path("Laser.wav");
                _V_shot_sound->load();
                _V_shot_sound->set_volume(0.5);
                _V_shot_sound->play();
                _V_list_to_destroy.push_back(_V_shot_sound);
                float angle=LL::sexagesimal_to_radian(-90);
                shots.push_back(Weapon_Shot(0,Direction_Shot(LL_MathStructure::create_point(_V_position[0],
                                                                                            _V_position[1],
                                                                                            _V_position[2]),angle)));
                _V_timer.clear();
            }
        }
        void draw()
        {
            if(_V_timer_temp.get_time()>=2)
            {
                if(_V_list_to_destroy.size())
                {
                    auto iter=_V_list_to_destroy.begin();
                    delete(*iter);
                    _V_list_to_destroy.erase(iter);
                }
                _V_timer_temp.clear();
                _V_timer_temp.play();
            }
            if(_V_giants_shots)
                _V_bullet_size=GIANT_SHOT_SIZE;
            else
                _V_bullet_size=SHOT_SIZE;
            if(_V_fast_shot)
                _V_time_to_shot=TIME_TO_SHOT_INC;
            else
                _V_time_to_shot=TIME_TO_SHOT;
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
                    glutSolidSphere(_V_bullet_size,4,4);
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
            for(auto iter=_V_list_to_destroy.begin();iter!=_V_list_to_destroy.end();++iter)
                delete(*iter);
            _V_list_to_destroy.clear();
            _V_timer.clear();
            shots.clear();
            _V_giants_shots=false;
            _V_timer_giants_shots.stop();
            _V_fast_shot=false;
            _V_timer_fast_shot.stop();
        }
        void message()
        {
            if(_V_giants_shots)
                std::cout<<"GIANT SHOTS!"<<std::endl;
            if(_V_fast_shot)
                std::cout<<"FAST SHOTS!"<<std::endl;
        }
        bool enhancer_collision(Enhancer* enhancer)
        {
            if(LL::segment_collision(enhancer->getPosX()-enhancer->get_size()*COLLISION_TOLERANCE/2,
                                     enhancer->getPosX()+enhancer->get_size()*COLLISION_TOLERANCE/2,
                                     _V_position[0]-_V_size/2,_V_position[0]+_V_size/2))
            {
                if(LL::segment_collision(enhancer->getPosY()-enhancer->get_size()*COLLISION_TOLERANCE/2,
                                         enhancer->getPosY()+enhancer->get_size()*COLLISION_TOLERANCE/2,
                                                 _V_position[1]-_V_size/2,_V_position[1]+_V_size/2))
                {
                    if(enhancer->get_type()==ENHANCER_GIANT_SHOT)
                    {
                        _V_timer_giants_shots.stop();
                        _V_timer_giants_shots.play();
                        _V_giants_shots=true;
                    }
                    if(enhancer->get_type()==ENHANCER_FAST_SHOT)
                    {
                        _V_timer_fast_shot.stop();
                        _V_timer_fast_shot.play();
                        _V_fast_shot=true;
                    }
                    return true;
                }
            }
            return false;
        }
        int meteor_collision(meteor& meteoro)
        {
            for(std::list<Weapon_Shot>::iterator iter=shots.begin();iter!=shots.end();++iter)
            {
                float pos_x=((*iter).second).first[0]+((*iter).first*cos(((*iter).second).second));
                float pos_y=((*iter).second).first[1];
                float pos_z=((*iter).second).first[2]+((*iter).first*sin(((*iter).second).second));
                if(LL::segment_collision(meteoro.getPosX()-meteoro.get_size()/2,meteoro.getPosX()+meteoro.get_size()/2,
                                         pos_x-_V_bullet_size/2,pos_x+_V_bullet_size/2))
                {
                    if(LL::segment_collision(meteoro.getPosY()-meteoro.get_size()*COLLISION_TOLERANCE/2,
                                             meteoro.getPosY()+meteoro.get_size()*COLLISION_TOLERANCE/2,
                                             pos_y-_V_bullet_size/2,pos_y+_V_bullet_size/2))
                    {
                        if(LL::segment_collision(meteoro.getPosZ()-meteoro.get_size()*COLLISION_TOLERANCE/2,
                                             meteoro.getPosZ()+meteoro.get_size()*COLLISION_TOLERANCE/2,
                                             pos_z-_V_bullet_size/2,pos_z+_V_bullet_size/2))
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
            clear();
        }
};

#endif // INCLUDED_PLAYER_H

