#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} Bundle;

typedef struct {
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;
} Triangle_3d;

typedef struct {
    float a, i, j, k;
} Quat;

typedef struct {
    float x, y, z;
    Quat *rotation;
} Camera;

typedef struct {
    float x, y, z, xvel, yvel, zvel;
} Player;

typedef struct {
    int seed, instruction_length;
    int *instruction_lengths;
    int8_t **instructions;
} Species;

typedef struct {
    int current_form_index, current_size;
    float seed_x, seed_y, seed_z;
    int8_t *current_form;
    Species *species;
} Plant;

typedef struct {
    float *items;
    int top;
    int capacity;
} FloatStack;

typedef struct {
    float x, y, z;
} PlantVertex;
typedef struct {
    float x1, y1, z1, x2, y2, z2;
} PlantEdge;

#endif