#include "inputs.h"

#include <math.h>

#include "game.h"

/*Global to detect in between key press and key release*/
int key_states[NUM_KEYS] = {0};
int key_typed_states[NUM_KEYS] = {0};
int lock_cursor = 0;
int mouse_position[2] = {0, 0};

void toggle_cursor_lock() {
    lock_cursor = (lock_cursor + 1) % 2;
    SDL_SetRelativeMouseMode(lock_cursor);
}

void get_key_states(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                key_states[key_W] = 1;
                key_typed_states[key_W] = 1;
                break;
            case SDLK_a:
                key_states[key_A] = 1;
                key_typed_states[key_A] = 1;
                break;
            case SDLK_s:
                key_states[key_S] = 1;
                key_typed_states[key_S] = 1;
                break;
            case SDLK_d:
                key_states[key_D] = 1;
                key_typed_states[key_D] = 1;
                break;
            case SDLK_SPACE:
                key_states[key_SPACE] = 1;
                key_typed_states[key_SPACE] = 1;
                break;
            case SDLK_ESCAPE:
                key_states[key_ESC] = 1;
                key_typed_states[key_ESC] = 1;
                break;
            default:
                break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                key_states[key_W] = 0;
                key_typed_states[key_W] = -1;
                break;
            case SDLK_a:
                key_states[key_A] = 0;
                key_typed_states[key_A] = -1;
                break;
            case SDLK_s:
                key_states[key_S] = 0;
                key_typed_states[key_S] = -1;
                break;
            case SDLK_d:
                key_states[key_D] = 0;
                key_typed_states[key_D] = -1;
                break;
            case SDLK_SPACE:
                key_states[key_SPACE] = 0;
                key_typed_states[key_SPACE] = -1;
                break;
            case SDLK_ESCAPE:
                key_states[key_ESC] = 0;
                key_typed_states[key_ESC] = -1;
                break;
            default:
                break;
        }
    }
}

void get_mouse_position(SDL_Event event) {
    if (lock_cursor) {
        if (event.type == SDL_MOUSEMOTION) {
            mouse_position[0] += event.motion.xrel;
            mouse_position[1] += event.motion.yrel;
        }
        if (mouse_position[0] < 0) {
            mouse_position[0] = SCREEN_WIDTH;
        } else if (mouse_position[0] > SCREEN_WIDTH) {
            mouse_position[0] = 0;
        }
        if (mouse_position[1] < 0) {
            mouse_position[1] = SCREEN_HEIGHT;
        } else if (mouse_position[1] > SCREEN_HEIGHT) {
            mouse_position[1] = 0;
        }
    }
}

void handle_mouse_movement() {
    /*Convert mouse x into horizontal rotation and mouse y into vertical rotation.*/
    float lateral_rotation =
        (((float)(mouse_position[0]) - ((float)SCREEN_WIDTH / 2)) / ((float)SCREEN_WIDTH)) * 2 * PI;

    if (lateral_rotation < 0) {
        lateral_rotation = 2 * PI + lateral_rotation;
    }
    float vertical_rotation =
        (((float)(mouse_position[1]) - ((float)SCREEN_HEIGHT / 2)) / ((float)SCREEN_HEIGHT)) * PI;

    if (vertical_rotation < 0) {
        vertical_rotation = 2 * PI + vertical_rotation;
    }

    // rotate left/right:
    set_game_camera_angle(lateral_rotation, 0, 1, 0);
    set_horizontal_rotation(lateral_rotation);
    // rotate up/down:
    move_camera(0, 0, 0, vertical_rotation, -1, 0, 0);
}

void handle_key_presses() {
    int pressed = 0;
    int d_fb, d_ud, d_lr;
    /*left-right, up-down, forward-backward.*/
    d_lr = 0;
    d_ud = 0;
    d_fb = 0;

    if (key_typed_states[key_ESC] == 1) {
        toggle_cursor_lock();
    }
    if (key_states[key_W]) {
        d_fb = 1;
        pressed = 1;
    }
    if (key_states[key_S]) {
        d_fb = -1;
        pressed = 1;
    }
    if (key_states[key_A]) {
        d_lr = -1;
        pressed = 1;
    }
    if (key_states[key_D]) {
        d_lr = 1;
        pressed = 1;
    }
    // printf("lr: %d, fb: %d\n", d_lr, d_fb);
    if (pressed) {
        set_player_velocity(d_lr, d_ud, d_fb, PLAYER_VELOCITY);
    } else {
        set_player_velocity(0, 0, 0, 0);
    }
    // printf("player velocity: (%f, %f, %f)\n", player->xvel, player->yvel, player->zvel);
    move_player();
    // printf("player coordinates: (%f, %f, %f)\n", player->x, player->y, player->z);
}

void handle_inputs() {
    /*printf("key states: W: %d, A: %d, S: %d, D: %d, SPACE: %d\n", key_states[key_W],
           key_states[key_A], key_states[key_S], key_states[key_D], key_states[key_SPACE]);*/
    SDL_Event event;
    /*Set all the typed key states to 0*/
    for (int i = 0; i < NUM_KEYS; i++) {
        key_typed_states[i] = 0;
    }
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit_game();
                exit(0);
                break;

            default:
                break;
        }
        get_key_states(event);
        get_mouse_position(event);
    }
    handle_key_presses();
    handle_mouse_movement();
}