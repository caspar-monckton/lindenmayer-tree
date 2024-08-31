#include "camera.h"

#include "quaternions.h"

void set_camera_position(Camera *camera, float x, float y, float z) {
    camera->x = x;
    camera->y = y;
    camera->z = z;
}

void set_camera_angle(Camera *camera, float angle, float x, float y, float z) {
    euler_to_quat(camera->rotation, angle, x, y, z);
}
void rotate_camera(Camera *camera, float angle, float x, float y, float z) {
    Quat new_rotation;
    euler_to_quat(&new_rotation, angle, x, y, z);
    q_mul(camera->rotation, camera->rotation, &new_rotation);
}
void translate_camera(Camera *camera, float dx, float dy, float dz) {
    camera->x += dx;
    camera->y += dy;
    camera->z += dz;
}