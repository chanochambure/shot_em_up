#ifndef INCLUDED_BACKGROUNG_H
#define INCLUDED_BACKGROUNG_H

#include "../main_headers.h"

struct background{

private: 
    LL_AL5::Image _V_sprite_background;
    GLuint _V_texture_background = 0;

public:
	background(){
		_V_sprite_background.set_path("space.png");
        _V_sprite_background.load();
        _V_texture_background = al_get_opengl_texture(_V_sprite_background);
    
	}
    
    void draw(){
    	glPushMatrix();
	        GLfloat nave_ambient[] = { 0.0, 0.0, 0.0, 1.0};
	        GLfloat nave_diffuse[] = { 0.001, 0.001, 0.001, 1.0 };
	        GLfloat	nave_specular[] = { 0.0002, 0.002, 0.002, 1.0 };
	        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, nave_ambient);
	        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, nave_diffuse);
	        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, nave_specular);

            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D,_V_texture_background);
            glBegin(GL_QUADS);

                glTexCoord2f(0.0f,0.0f);
                glVertex3f(MIN_ENEMY_X * CONST_WINDOWS,MAX_ENEMY_Y * CONST_WINDOWS,MIN_ENEMY_Z);

                glTexCoord2f(0.0f,1.0f);
                glVertex3f(MIN_ENEMY_X * CONST_WINDOWS,MIN_ENEMY_Y * CONST_WINDOWS,MIN_ENEMY_Z);
                
                glTexCoord2f(1.0f,1.0f);
                glVertex3f(MAX_ENEMY_X * CONST_WINDOWS,MIN_ENEMY_Y * CONST_WINDOWS,MIN_ENEMY_Z);
                
                glTexCoord2f(1.0f,0.0f);
                glVertex3f(MAX_ENEMY_X * CONST_WINDOWS,MAX_ENEMY_Y * CONST_WINDOWS,MIN_ENEMY_Z);
                

            glEnd();


                glDisable(GL_TEXTURE_2D);
            glPopMatrix();
            glEnable(GL_LIGHTING);
    }


};

#endif // INCLUDED_BACKGROUND_H
