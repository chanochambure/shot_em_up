#ifndef INCLUDED_BLOCK_H
#define INCLUDED_BLOCK_H

class Block
{
    private:
        GLfloat _V_size_x;
        GLfloat _V_size_y;
        GLfloat _V_size_z;
    public:
        Block(GLfloat size_x,GLfloat size_y,GLfloat size_z)
        {
            _V_size_x=size_x;
            _V_size_y=size_y;
            _V_size_z=size_z;
        }
        void draw()
        {
            glPushMatrix();
            glBegin(GL_LINE_LOOP);
                //glColor3d(0,255,0);
                glVertex3f(-_V_size_x/2,-_V_size_y/2,-_V_size_z/2);
                glVertex3f(-_V_size_x/2,-_V_size_y/2,_V_size_z/2);
                glVertex3f(-_V_size_x/2,_V_size_y/2,_V_size_z/2);
                glVertex3f(-_V_size_x/2,_V_size_y/2,-_V_size_z/2);
            glEnd();
            glBegin(GL_LINE_LOOP);
                //glColor3d(0,0,255);
                glVertex3f(_V_size_x/2,-_V_size_y/2,-_V_size_z/2);
                glVertex3f(_V_size_x/2,-_V_size_y/2,_V_size_z/2);
                glVertex3f(_V_size_x/2,_V_size_y/2,_V_size_z/2);
                glVertex3f(_V_size_x/2,_V_size_y/2,-_V_size_z/2);
            glEnd();
            glBegin(GL_LINES);
                //glColor3d(255,0,0);
                glVertex3f(-_V_size_x/2,-_V_size_y/2,-_V_size_z/2);
                glVertex3f(_V_size_x/2,-_V_size_y/2,-_V_size_z/2);

                glVertex3f(-_V_size_x/2,-_V_size_y/2,_V_size_z/2);
                glVertex3f(_V_size_x/2,-_V_size_y/2,_V_size_z/2);

                glVertex3f(-_V_size_x/2,_V_size_y/2,_V_size_z/2);
                glVertex3f(_V_size_x/2,_V_size_y/2,_V_size_z/2);

                glVertex3f(-_V_size_x/2,_V_size_y/2,-_V_size_z/2);
                glVertex3f(_V_size_x/2,_V_size_y/2,-_V_size_z/2);
            glEnd();
            glPopMatrix();
        }
};

#endif // INCLUDED_BLOCK_H

