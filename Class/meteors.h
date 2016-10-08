#ifndef INCLUDED_METEORS_H
#define INCLUDED_METEORS_H

#include "../main_headers.h"

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

class meteors {
public:
	meteors(){
		position.x = 0;
		position.y = 0;
		position.z = 0;

		size = 1;
		direction = 0;
		speed = 0.1;
	}

	meteors(GLfloat x, GLfloat y, GLfloat sizeM, GLfloat dir, GLfloat speedM){
		position.x = x;
		position.y = y;
		position.z = 0;

		size = sizeM;
		direction = dir;
		speed = speedM;
	}

	meteors(GLfloat x, GLfloat y, GLfloat z, GLfloat sizeM, GLfloat dir, GLfloat speedM){
		position.x = x;
		position.y = y;
		position.z = z;

		size = sizeM;
		direction = dir;
		speed = speedM;
	}
	virtual ~meteors(){}

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

private:
	point position;
	GLfloat direction;
	GLfloat size;
	GLfloat speed;
};

struct meteorsEngine {
	meteorsEngine(){
		for (int i = 0; i < 10; ++i) {
			meteors met;
			met.makeRandomPosition();
			meteorsInTheSpace.push_back(met);
		}
	}
	meteorsEngine(int number_of_metoers){
		for (int i = 0; i < number_of_metoers; ++i) {
			meteors met;
			met.makeRandomPosition();
			meteorsInTheSpace.push_back(met);
		}
	}
	meteorsEngine(const meteorsEngine & engineMet){
		for (int i = 0; i < engineMet.meteorsInTheSpace.size(); ++i) {
			meteorsInTheSpace.push_back( engineMet.meteorsInTheSpace[i] );
		}
	}
	virtual ~meteorsEngine(){
		for (int i = 0; i < meteorsInTheSpace.size(); ++i){
			meteorsInTheSpace.pop_back();
		}
	}

	void checkingIfAlive(){
		// checking if the metoers are still alive
		std::vector<int> positionM;
		for (int i = 0; i < meteorsInTheSpace.size(); ++i){
			if ( meteorsInTheSpace[i].stillAlive() ){
				positionM.push_back(i);
			}
		}
		// Erasing metoers which are far away from the windows
		for (int i = (positionM.size()-1); i >= 0 ; --i){
			meteorsInTheSpace.erase( meteorsInTheSpace.begin() + i );
		}
	}

	void move(){
		for (int i = 0; i < meteorsInTheSpace.size(); ++i){
			meteorsInTheSpace[i].move();
		}
		checkingIfAlive();
	}
	void draw(){
		for (int i = 0; i < meteorsInTheSpace.size(); ++i){
			meteorsInTheSpace[i].draw();
		}
		move();
	}

	std::vector< meteors > meteorsInTheSpace;
};

#endif // INCLUDED_METEORS_H
