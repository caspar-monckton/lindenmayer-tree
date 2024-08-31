#include "shared.h"

#ifndef CAMERA_C_
#define CAMERA_C_

void set_camera_position(Camera *camera, float x, float y, float z);
void translate_camera(Camera *camera, float dx, float dy, float dz);
/*x, y, z reprent the axis that the camera is rotating about and should be normalised*/
void set_camera_angle(Camera *camera, float angle, float x, float y, float z);
void rotate_camera(Camera *camera, float angle, float x, float y, float z);

#endif