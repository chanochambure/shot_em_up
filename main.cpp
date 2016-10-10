#include "gamemain.h"

int main()
{
    LL_AL5::init_allegro();
    LL_AL5::image_addon();
    LL_AL5::text_addon();
    LL_AL5::audio_addon();
    LL_AL5::Display display(SIZE_X,SIZE_Y);
    display.set_flag(ALLEGRO_OPENGL);

    LL_AL5::Timer timer;
    timer.set_speed_seconds(1.0/30);
    timer.create();
    timer.start();

    LL_AL5::Input input;
    input.register_display(display);
    input.register_timer(timer);
    input.keyboard_on();
    input.mouse_on(true);
    start_game(&display,&input);
    LL_AL5::uninstall_audio();
	return 0;
}
