#include "shared.h"

#ifndef PROCESS_3D_C
#define PROCES_3D_C

void rotate_point(float in_x, float in_y, float in_z, float *out_x, float *out_y, float *out_z,
                  float dx, float dy, float dz, Quat *rotation);

void transform_point(float in_x, float in_y, float in_z, float *out_x, float *out_y, float dx,
                     float dy, float dz, Quat *rotation, int *in_front);

float get_distance_to_point(float x1, float y1, float z1, float x2, float y2, float z2);

#endif