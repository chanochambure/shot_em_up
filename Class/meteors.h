#ifndef INCLUDED_METEOR_H
#define INCLUDED_METEOR_H

#include "../main_headers.h"
// Meteors
#define MAX_ANGLE 180
#define MIN_ANGLE -180
#define MAX_SPEED_EASY      300
#define MAX_SPEED_NORMAL    400
#define MAX_SPEED_HARD      500
#define MIN_SPEED           200
// Engine
#define MAX_METEORS_PEER_CREATE 20
#define MIN_METEORS         7
#define MAX_METEORS_EASY    100
#define MAX_METEORS_NORMAL  150
#define MAX_METEORS_HARD    200
#define TIME_TO_CREATE      0.6
#define SIZE_METEORS        40

class meteor {
    private:
        LL_MathStructure::Point<3> position;
        GLfloat direction;
        GLfloat size;
        GLfloat speed;
    public:
        meteor()
        {
            size = SIZE_METEORS;
            direction = LL::sexagesimal_to_radian( LL::random( MIN_ANGLE, MAX_ANGLE, true) );
            speed = MIN_SPEED;
        }

        void move(float dt=1){
            position[0] +=(dt*speed*cos(direction));
            position[1] +=(dt*speed*sin(direction));
            position[2] +=(dt*4*speed);
        }

        void draw(GLuint texture_id,GLUquadric* qobj){
            GLfloat ambient[] = { 0.5, 0.3, 0.0, 1.0};
            GLfloat diffuse[] = { 0.8, 0.6, 0.0, 1.0 };
            GLfloat	specular[] = { 1.0, 0.9, 0.0, 1.0 };
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            glPushMatrix();
                glTranslatef(position[0],position[1],position[2]);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,texture_id);
                gluSphere(qobj,size,8,8);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }

        bool stillAlive()
        {
            return ( ( position[0] < ( MAX_ENEMY_X + size ) ) && ( position[0] > ( MIN_ENEMY_X - size ) )
                  && ( position[1] < ( MAX_ENEMY_Y + size ) ) && ( position[1] > ( MIN_ENEMY_Y - size ) )
                  && ( position[2] < ( MAX_ENEMY_Z + size ) ) && ( position[2] > ( MIN_ENEMY_Z - size ) ) );
        }

        void makeRandomPosition(){
            position[0] = LL::random(-SIZE_X/2, SIZE_X/2, true);
            position[1] = LL::random(-SIZE_Y/2, SIZE_Y/2, true);
            position[2] = MIN_ENEMY_Z+size;
        }
        float getPosX(){
            return position[0];
        }
        float getPosY(){
            return position[1];
        }
        float getPosZ(){
            return position[2];
        }
        float get_size()
        {
            return size;
        }

        void setPosition(float x, float y,float z){
            position[0] = x;
            position[1] = y;
            position[2] = z;
        }
};

struct meteorsEngine {
    private:
        std::list<meteor> meteorsInTheSpace;
        double timerForMeteors;
        unsigned int numberOfMaxMeteors;
        LL::Chronometer timer;
        LL_AL5::Image _V_sprite_meteor;
        GLuint _V_texture_meteor = 0;
        GLUquadric *_V_qobj;
    public:
        meteorsEngine(unsigned int numMaxMeteors = MAX_METEORS_EASY, double time_meteor = TIME_TO_CREATE)
        {
            numberOfMaxMeteors = numMaxMeteors;
            timerForMeteors = time_meteor;
            _V_sprite_meteor.set_path("meteor.png");
            _V_sprite_meteor.load();
            _V_texture_meteor = al_get_opengl_texture(_V_sprite_meteor);
            timer.play();
            _V_qobj = gluNewQuadric();
            gluQuadricTexture(_V_qobj,GL_TRUE);
        }
        void setTimerForMeteors(double timer)
        {
            timerForMeteors = timer;
        }
        void setLimitOfMeteorsCreate(unsigned int limit){
            numberOfMaxMeteors = limit;
        }
        void makeMeteors(int numberOfMeteors = 1){
            for (int i = 0; i < numberOfMeteors; ++i){
                meteor met;
                met.makeRandomPosition();
                meteorsInTheSpace.push_back(met);
            }
        }

        void checkingIfAlive()
        {
            for(std::list<meteor>::iterator iter=meteorsInTheSpace.begin();iter!=meteorsInTheSpace.end();)
            {
                if(!((*iter).stillAlive()))
                    iter=meteorsInTheSpace.erase(iter);
                else
                    ++iter;
            }
        }
        void move(float dt){
            if(timer.get_time()>=timerForMeteors or meteorsInTheSpace.size()<=MIN_METEORS)
            {
                makeMeteors( LL::random(MIN_METEORS, MAX_METEORS_PEER_CREATE, true) );
                timer.clear();
            }
            for (std::list< meteor >::iterator it = meteorsInTheSpace.begin(); it != meteorsInTheSpace.end(); ++it){
                (*it).move(dt);
            }
            checkingIfAlive();
        }
        void draw()
        {
            for (std::list< meteor >::iterator it = meteorsInTheSpace.begin(); it != meteorsInTheSpace.end(); ++it)
                (*it).draw(_V_texture_meteor,_V_qobj);
        }
        template<typename T>
        int meteor_collision(T* player)
        {
            int destroyed=0;
            for (std::list< meteor >::iterator it = meteorsInTheSpace.begin(); it != meteorsInTheSpace.end();)
            {
                switch(player->meteor_collision(*it))
                {
                    case 1:
                    {
                        it=meteorsInTheSpace.erase(it);
                        ++destroyed;
                        break;
                    }
                    case 2:
                        return -1;
                    default:
                        ++it;
                }
            }
            return destroyed;
        }
        void clear()
        {
            timer.clear();
            meteorsInTheSpace.clear();
        }
        ~meteorsEngine(){
            gluDeleteQuadric(_V_qobj);
            meteorsInTheSpace.clear();
            glDeleteTextures(1, &_V_texture_meteor);
        }
};

#endif // INCLUDED_METEOR_H
