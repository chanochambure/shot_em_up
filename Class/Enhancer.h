#ifndef ENHANCER_INCLUDED
#define ENHANCER_INCLUDED

#define ENHANCER_GIANT_SHOT 1
#define ENHANCER_FAST_SHOT 2
#define ENHANCER_DURATION_IN_DISPLAY 2

#define TIMER_ENHANCER_ENGINE 6
#define ENHANCER_SIZE 40

class Enhancer
{
    private:
        int type=0;
        bool activated=0;
        LL_MathStructure::Point<2> _V_position;
        GLfloat _V_size=ENHANCER_SIZE;
        LL_AL5::Image _V_sprite;
        GLuint _V_texture = 0;
        LL::Chronometer timer;
    public:
        Enhancer()
        {
        }
        int get_type()
        {
            return type;
        }
        void set_type(int option)
        {
            type=option;
        }
        void load()
        {
            if(type==ENHANCER_GIANT_SHOT)
            {
                _V_sprite.set_path("giant_shot.png");
                _V_sprite.load();
                _V_texture = al_get_opengl_texture(_V_sprite);
            }
            if(type==ENHANCER_FAST_SHOT)
            {
                _V_sprite.set_path("fast_shot.png");
                _V_sprite.load();
                _V_texture = al_get_opengl_texture(_V_sprite);
            }
        }
        void set_position(float x,float y)
        {
            _V_position[0]=x;
            _V_position[1]=y;
        }
        void activate()
        {
            activated=true;
            timer.play();
            _V_position[0] = LL::random(-SIZE_X/2+_V_size, SIZE_X/2-_V_size, true);
            _V_position[1] = LL::random(-SIZE_Y/2+_V_size, SIZE_Y/2-_V_size, true);
        }
        float get_size()
        {
            return _V_size;
        }
        float getPosX(){
            return _V_position[0];
        }
        float getPosY(){
            return _V_position[1];
        }
        void desactivate()
        {
            activated=false;
            timer.stop();
        }
        bool is_activated()
        {
            return activated;
        }
        void move()
        {
            if(activated)
            {
                if(timer.get_time()>=ENHANCER_DURATION_IN_DISPLAY)
                    desactivate();
            }
        }
        void draw()
        {
            if(activated)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,_V_texture);
                glBegin(GL_QUADS);
                    glNormal3f(0,0,-1);
                    glTexCoord2f(0,0);
                    glVertex3f(_V_position[0]+_V_size/2,_V_position[1]+_V_size/2,INITIAL_Z);
                    glTexCoord2f(1,0);
                    glVertex3f(_V_position[0]-_V_size/2,_V_position[1]+_V_size/2,INITIAL_Z);
                    glTexCoord2f(1,1);
                    glVertex3f(_V_position[0]-_V_size/2,_V_position[1]-_V_size/2,INITIAL_Z);
                    glTexCoord2f(0,1);
                    glVertex3f(_V_position[0]+_V_size/2,_V_position[1]-_V_size/2,INITIAL_Z);
                glEnd();
                glDisable(GL_TEXTURE_2D);
            }
        }
};

class EnhancerEngine
{
    private:
        LL::Chronometer timer;
        Enhancer _V_giant_bullets;
        Enhancer _V_fast_bullets;
    public:
        EnhancerEngine()
        {
            _V_giant_bullets.set_type(ENHANCER_GIANT_SHOT);
            _V_giant_bullets.load();
            _V_fast_bullets.set_type(ENHANCER_FAST_SHOT);
            _V_fast_bullets.load();
            timer.play();
        }
        void move()
        {
            if(timer.get_time()>=TIMER_ENHANCER_ENGINE)
            {
                int choose=LL::random(1,4);
                if(choose==ENHANCER_GIANT_SHOT)
                    _V_giant_bullets.activate();
                else if(choose==ENHANCER_FAST_SHOT)
                    _V_fast_bullets.activate();
                timer.stop();
                timer.play();
            }
        }
        void draw()
        {
            _V_giant_bullets.draw();
            _V_fast_bullets.draw();
        }
        template<typename T>
        int enhancer_collision(T* player)
        {
            if(_V_giant_bullets.is_activated())
            {
                if(player->enhancer_collision(&_V_giant_bullets))
                    _V_giant_bullets.desactivate();
            }
            if(_V_fast_bullets.is_activated())
            {
                if(player->enhancer_collision(&_V_fast_bullets))
                    _V_fast_bullets.desactivate();
            }
            return 0;
        }
        void clear()
        {
            timer.clear();
            _V_giant_bullets.desactivate();
            _V_fast_bullets.desactivate();
        }
        ~EnhancerEngine()
        {
        }
};

#endif // ENHANCER_INCLUDED
