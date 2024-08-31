#include "main.h"

#include <stdio.h>

#include "draw.h"
#include "game.h"
#include "init.h"
#include "inputs.h"
#include "shared.h"

int main(int argc, char* argv[]) {
    Bundle app;

    init_SDL(&app);
    init_game();

    while (1) {
        update_game();
        handle_inputs();
        draw_scene(&app);
        SDL_Delay(15);
    }
    return 69;
}