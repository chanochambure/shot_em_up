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
#define SIZE_METEORS        20

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

        void drawCircle(GLdouble xPoint, GLdouble yPoint, GLdouble radio){
            glBegin(GL_POLYGON);
                glColor3d(0,0,255); //Color RGB
                GLdouble dividedAngle;
                dividedAngle = (2 * LL::MATH_PI) / 50;
                GLdouble angle;
                for(GLint numPoints = 0; numPoints < 50; ++numPoints){
                    angle = numPoints * dividedAngle ;
                    glVertex3f( xPoint + ( radio * cos(angle) ), yPoint + ( radio * sin(angle) ), 0);
                }
            glEnd();
        }

        void draw(){
            glPushMatrix();
                drawCircle(position[0],position[1], size);
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
    public:
        meteorsEngine(unsigned int numMaxMeteors = MAX_METEORS_EASY, double time_meteor = TIME_TO_CREATE){
            numberOfMaxMeteors = numMaxMeteors;
            timerForMeteors = time_meteor;
            timer.play();
        }
    /*
        meteorsEngine(const meteorsEngine & engineMet){
            for (int i = 0; i < engineMet.meteorsInTheSpace.size(); ++i) {
                meteorsInTheSpace.push_back( engineMet.meteorsInTheSpace[i] );
            }
        }
    */
        /*void run(){
            while( meteorsInTheSpace.size() > 0 ){
                if (timer.get_time() >= timerForMeteors){
                    makeMeteors( LL::random(MIN_METEORS, numberOfMaxMeteors, true) );
                    timer.clear();
                }
                move();
                checkingIfAlive();
                std::cout<<meteorsInTheSpace.size()<<'\n'<<'\n';
            }
        }*/
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
                (*it).draw();
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
        }
};

#endif // INCLUDED_METEOR_H
