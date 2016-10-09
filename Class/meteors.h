#ifndef INCLUDED_METEOR_H
#define INCLUDED_METEOR_H

#include "../main_headers.h"
// Meteors
#define MAX_ANGLE 120
#define MIN_ANGLE 60
#define MAX_SPEED 10
#define MIN_SPEED 1
// Engine
#define MAX_METEORS 50
#define MIN_METEORS 1
#define TIME_TO_CREATE 0.5 //0.00013

struct point {
	point(){
		x = y = z = 0;
	}
	point(float a, float b){
		x = a;
		y = b;
		z = 0;
	}
	point(float a, float b, float c){
		x = a;
		y = b;
		z = c;
	}
	point(const point & p){
		x = p.x;
		y = p.y;
		z = p.z;
	}
	virtual ~point(){}
	
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class meteor {
public:
	meteor(){
		position.x = 0;
		position.y = 0;
		position.z = 0;
		size = 1;
		direction = LL::sexagesimal_to_radian( LL::random( MIN_ANGLE, MAX_ANGLE, true) );
		speed = LL::random(MIN_SPEED, MAX_SPEED, true);
	}
	meteor(GLfloat x, GLfloat y, GLfloat sizeM, GLfloat dir, GLfloat speedM){
		position.x = x;
		position.y = y;
		position.z = 0;
		size = sizeM;
		direction = dir;
		speed = speedM;
	}
	meteor (GLfloat x, GLfloat y, GLfloat z, GLfloat sizeM, GLfloat dir, GLfloat speedM){
		position.x = x;
		position.y = y;
		position.z = z;
		size = sizeM;
		direction = dir;
		speed = speedM;
	}
	meteor (const meteor & met){
		position.x = met.getPosX();
		position.y = met.getPosY();
		position.z = met.getPosZ();
		size = met.size;
		direction = met.direction;
		speed = met.direction;
	}

//	virtual ~meteor(){}

	void move(){
		position.x = ( position.x + ( speed*cos(direction) ) );
		position.y = ( position.y + ( speed*sin(direction) ) );
	}

	void drawCircle(GLdouble xPoint, GLdouble yPoint, GLdouble radio){
		glBegin(GL_LINE_LOOP);
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
			drawCircle(position.x, position.y, size);
		glPopMatrix();
	}

	bool stillAlive(){
		return ( ( position.x < ( SIZE_X + size ) ) && ( position.x > ( 0 - size ) )  && ( position.y < ( SIZE_Y + size ) ) && ( position.y > ( 0 - size ) ) );
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
		makeRandomPosition1F();
	}	
	// Posicion Generada opuesta a la linea de partida
	void makeRandomPosition1F(){
		position.x = LL::random(0, SIZE_X, true);
		position.y = 0;
	}
	void printPosition(){
		std::cout<<"position: "<<position.x<<" <-> "<<position.y<<" <-> "<<'\n';
	}
	float getPosX() const{
		return position.x;
	}
	float getPosY() const{
		return position.y;
	}
	float getPosZ() const{
		return position.z;
	}
	void setPosition(float x, float y){
		position.x = x;
		position.y = y;
	}

private:
	point position;
	GLfloat direction;
	GLfloat size;
	GLfloat speed;
};

struct meteorsEngine {

	meteorsEngine(int numberOfMeteors = -1, int numMaxMeteors = MAX_METEORS, double timer = TIME_TO_CREATE){
		numberOfMeteors = (numberOfMeteors == -1)?LL::random(MIN_METEORS, MAX_METEORS,true):numberOfMeteors;
		numberOfMaxMeteors = numMaxMeteors;
		timerForMeteors = timer;
		makeMeteors(numberOfMeteors);
		run();
	}
/*
	meteorsEngine(const meteorsEngine & engineMet){
		for (int i = 0; i < engineMet.meteorsInTheSpace.size(); ++i) {
			meteorsInTheSpace.push_back( engineMet.meteorsInTheSpace[i] );
		}
	}
*/
	virtual ~meteorsEngine(){
		for (int i = 0; i < meteorsInTheSpace.size(); ++i){
			meteorsInTheSpace.pop_back();
		}
	}

	void run(){
	    LL::Chronometer timer;
	    timer.play();
		while( meteorsInTheSpace.size() > 0 ){
			if (timer.get_time() >= timerForMeteors){
				makeMeteors( LL::random(MIN_METEORS, numberOfMaxMeteors, true) );
				timer.clear();
			}
			move();
			checkingIfAlive();
			std::cout<<meteorsInTheSpace.size()<<'\n'<<'\n';
		}
	}
	void setTimerForMeteors(double timer){
		timerForMeteors = timer;
	}
	void setLimitOfMeteorsCreate(int limit){
		numberOfMaxMeteors = limit;
	}

private:

	void makeMeteors(int numberOfMeteors = 10){
		for (int i = 0; i < numberOfMeteors; ++i){
			meteor met;
			met.makeRandomPosition();
			meteorsInTheSpace.push_back(met);
		}
	}

	void checkingIfAlive(){
		// checking if the metoers are still alive
		std::vector<int> positionM;
		for (int i = 0; i < meteorsInTheSpace.size(); ++i){
			if ( !( ( *( meteorsInTheSpace.begin() + i ) ).stillAlive() ) ){
				positionM.push_back(i);
			}
		}
		for (int i = (positionM.size()-1); i >= 0 ; --i){
			meteorsInTheSpace.erase( meteorsInTheSpace.begin() + i );
		}
	}

	void move(){
		for (std::vector< meteor >::iterator it = meteorsInTheSpace.begin(); it != meteorsInTheSpace.end(); ++it){
			(*it).move();
		}
	}
	void draw(){
		for (std::vector< meteor >::iterator it = meteorsInTheSpace.begin(); it != meteorsInTheSpace.end(); ++it){
			(*it).draw();
		}
		move();
	}

	std::vector< meteor > meteorsInTheSpace;
	double timerForMeteors;
	int numberOfMaxMeteors;
};

#endif // INCLUDED_METEOR_H
