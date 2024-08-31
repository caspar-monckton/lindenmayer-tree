#include "draw.h"

#include "game.h"
#include "process_3d.h"

void draw_player(Bundle *app) {
    Player *player = get_player();
    float px, py, pz;
    px = player->x;
    py = player->y;
    pz = player->z;
    // printf("\033[2J \033[H Player coordinates: (x, y, z) = (%f, %f, %f)\n", px, py, pz);

    SDL_Rect temp_rect;
    temp_rect.x = px + 100;
    temp_rect.y = pz + 100;
    temp_rect.w = 20 + py / 10;
    temp_rect.h = 20 + py / 10;
    SDL_RenderFillRect(app->renderer, &temp_rect);
}

void draw_point_2d(Bundle *app, float x, float y, int size) {
    SDL_Rect p_rect = {x + 100, y + 100, size, size};
    SDL_RenderFillRect(app->renderer, &p_rect);
}

void draw_point(Bundle *app, float x, float y, float z, int size) {
    float px, py, pz;
    int in_front;
    Player *player = get_player();
    Camera *camera = get_camera();
    px = player->x;
    py = player->y;
    pz = player->z;

    float proj_x, proj_y;
    transform_point(x, y, z, &proj_x, &proj_y, px, py, pz, camera->rotation, &in_front);
    if (in_front) {
        SDL_Rect p_rect = {proj_x + SCREEN_WIDTH / 2, proj_y + SCREEN_HEIGHT / 2, size, size};
        SDL_RenderFillRect(app->renderer, &p_rect);
    }
}

void draw_line(Bundle *app, float start_x, float start_y, float start_z, float end_x, float end_y,
               float end_z, int point_density) {
    float px, py, pz;
    Player *player = get_player();
    px = player->x;
    py = player->y;
    pz = player->z;

    /*figure out how many points to plot*/
    float dist = get_distance_to_point(0.5 * (start_x + end_x), 0.5 * (start_y + end_y),
                                       0.5 * (start_z + end_z), px, py, pz);
    int num_points = (int)((10 / dist) * ((float)(point_density)));
    // printf("\ndistance: %f\n", dist);
    /*linearly interpolate by drawing points at evenly spaced intervals between endpoints*/
    draw_point(app, start_x, start_y, start_z, DEFAULT_DOT_SIZE);
    for (int point_ind = 1; point_ind < num_points; point_ind++) {
        float t = (((float)point_ind) / ((float)num_points));
        float t_x = (start_x) * (1 - t) + (end_x)*t;
        float t_y = (start_y) * (1 - t) + (end_y)*t;
        float t_z = (start_z) * (1 - t) + (end_z)*t;

        draw_point(app, t_x, t_y, t_z, LINE_DOT_SIZE);
    }
    draw_point(app, end_x, end_y, end_z, DEFAULT_DOT_SIZE);
}

void draw_game_plant(Bundle *app, int plant_index) {
    PlantEdge *buffer = get_edge_buffer(plant_index);
    int buffer_length = get_edge_buffer_length(plant_index);
    for (int i = 0; i < buffer_length; i++) {
        draw_line(app, buffer[i].x1, buffer[i].y1, buffer[i].z1, buffer[i].x2, buffer[i].y2,
                  buffer[i].z2, LINE_DETAIL);
    }
}

void draw_objects(Bundle *app) {
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 255, 255);
    draw_player(app);
    SDL_SetRenderDrawColor(app->renderer, 0, 200, 100, 255);
    for (int i = 0; i < get_num_plants(); i++) {
        draw_game_plant(app, i);
    }
}

void draw_scene(Bundle *app) {
    SDL_SetRenderDrawColor(app->renderer, 86, 128, 255, 255);
    SDL_RenderClear(app->renderer);
    draw_objects(app);
    SDL_RenderPresent(app->renderer);
}