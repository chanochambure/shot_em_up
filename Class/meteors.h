#ifndef INCLUDED_METEOR_H
#define INCLUDED_METEOR_H

#include "../main_headers.h"
// Meteors
#define MAX_ANGLE 120
#define MIN_ANGLE 60
#define MAX_SPEED_EASY      300
#define MAX_SPEED_NORMAL    400
#define MAX_SPEED_HARD      500
#define MIN_SPEED           200
// Engine
#define MAX_METEORS_PEER_CREATE 3
#define MIN_METEORS         1
#define MAX_METEORS_EASY    10
#define MAX_METEORS_NORMAL  20
#define MAX_METEORS_HARD    30
#define TIME_TO_CREATE      1.5
#define SIZE_METEORS        30

class meteor {
    private:
        LL_MathStructure::Point<3> position;
        GLfloat direction;
        GLfloat size;
        GLfloat speed;
    public:
        meteor(){
            size = SIZE_METEORS;
            direction = LL::sexagesimal_to_radian( LL::random( MIN_ANGLE, MAX_ANGLE, true) );
            speed = MIN_SPEED;
        }
        meteor(GLfloat x, GLfloat y, GLfloat sizeM, GLfloat dir, GLfloat speedM){
            position[0] = x;
            position[1] = y;
            position[2] = 0;
            size = sizeM;
            direction = dir;
            speed = speedM;
        }
        meteor (GLfloat x, GLfloat y, GLfloat z, GLfloat sizeM, GLfloat dir, GLfloat speedM){
            position[0] = x;
            position[1] = y;
            position[2] = z;
            size = sizeM;
            direction = dir;
            speed = speedM;
        }
        meteor (const meteor & met){
            position=met.position;
            size = met.size;
            direction = met.direction;
            speed = met.speed;
        }

        void move(float dt=1){
            position[0] +=(dt*speed*cos(direction));
            position[1] +=(dt*speed*sin(direction));
        }

        void draw(GLuint texture_id){
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture_id);

                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
                glBegin(GL_QUADS);
                    glTexCoord2f(1, 1);
                    glVertex3f(position[0]-size/2,position[1]-size/2,0.0);

                    glTexCoord2f(0,1);
                    glVertex3f(position[0]+size/2,position[1]-size/2,0.0);

                    glTexCoord2f(0, 0);
                    glVertex3f(position[0]+size/2,position[1]+size/2,0.0);

                    glTexCoord2f(1, 0);
                    glVertex3f(position[0]-size/2,position[1]+size/2,0.0);
                glEnd();
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }

        bool stillAlive(){
            return ( ( position[0] < ( MAX_ENEMY_X + size ) ) && ( position[0] > ( MIN_ENEMY_X - size ) )
                  && ( position[1] < ( MAX_ENEMY_Y + size ) ) && ( position[1] > ( MIN_ENEMY_Y - size ) ) );
        }

        void changueDirection(GLfloat new_direction){
            direction = new_direction;
        }

        void changueSize(GLfloat new_size){
            size = new_size;
        }

        void changeSpeed(GLfloat new_speed){
            speed = new_speed;
        }
        void makeRandomPosition(){
            position[0] = LL::random(0, SIZE_X, true);
            position[1] = MIN_ENEMY_Y;
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

        void setPosition(float x, float y){
            position[0] = x;
            position[1] = y;
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
    public:
        meteorsEngine(unsigned int numMaxMeteors = MAX_METEORS_EASY, double time_meteor = TIME_TO_CREATE){
            numberOfMaxMeteors = numMaxMeteors;
            timerForMeteors = time_meteor;
            _V_sprite_meteor.set_path("meteor.png");
            _V_sprite_meteor.load();
            _V_texture_meteor = al_get_opengl_texture(_V_sprite_meteor);
            timer.play();
        }
        void setTimerForMeteors(double timer){
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

        void checkingIfAlive(){
            for(std::list<meteor>::iterator iter=meteorsInTheSpace.begin();iter!=meteorsInTheSpace.end();)
            {
                if(!((*iter).stillAlive()))
                    iter=meteorsInTheSpace.erase(iter);
                else
                    ++iter;
            }
        }

        void move(float dt){
            if(timer.get_time()>=timerForMeteors and meteorsInTheSpace.size()<=numberOfMaxMeteors)
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
                (*it).draw(_V_texture_meteor);
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
            meteorsInTheSpace.clear();
            glDeleteTextures(1, &_V_texture_meteor);
        }
};

#endif // INCLUDED_METEOR_H
