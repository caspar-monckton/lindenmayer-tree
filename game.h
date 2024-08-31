#include "shared.h"

#ifndef GAME_C_
#define GAME_C_

void move_camera(float x, float y, float z, float angle, float axis_x, float axis_y, float axis_z);
Camera *get_camera(void);

void move_player();
void set_player_velocity(int d_lr, int d_fb, int dy, float speed);
Player *get_player(void);
Plant *get_plant(int index);

void init_game(void);

void set_game_camera_angle(float angle, float x, float y, float z);
void set_horizontal_rotation(float angle);

void update_game(void);

PlantEdge *get_edge_buffer(int index);

int get_edge_buffer_length(int index);

void quit_game(void);

int get_num_plants(void);

#endif