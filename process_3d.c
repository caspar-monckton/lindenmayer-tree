#include "process_3d.h"

#include <math.h>

#include "quaternions.h"

// right handed coordinate system so facing -z.
void rotate_point(float in_x, float in_y, float in_z, float *out_x, float *out_y, float *out_z,
                  float dx, float dy, float dz, Quat *rotation) {
    float t_x, t_y, t_z;
    t_x = in_x - dx;
    t_y = in_y - dy;
    t_z = in_z - dz;

    // rotate point relative to camera rotation
    Quat quat_point = {0, t_x, t_y, t_z};
    Quat rotated_point;
    Quat inverse_rotation;

    // printf("\ncamera rotation: (%f, %f, %f, %f)\n", rotation->a, rotation->i, rotation->j,
    // rotation->k);
    q_neg(rotation, &inverse_rotation);
    // printf("\nconjugate camera rotation: (%f, %f, %f, %f)\n", inverse_rotation.a,
    // inverse_rotation.i, inverse_rotation.j, inverse_rotation.k);

    q_mul(&rotated_point, &inverse_rotation, &quat_point);
    q_mul(&quat_point, &rotated_point, rotation);
    t_x = quat_point.i;
    t_y = quat_point.j;
    t_z = quat_point.k;

    *out_x = t_x + dx;
    *out_y = t_y + dy;
    *out_z = t_z + dz;
}

void transform_point(float in_x, float in_y, float in_z, float *out_x, float *out_y, float dx,
                     float dy, float dz, Quat *rotation, int *in_front) {
    // printf("point start: (%f, %f, %f)", in_x, in_y, in_z);
    //  move point relative to camera position:
    float t_x, t_y, t_z;
    t_x = in_x - dx;
    t_y = in_y - dy;
    t_z = in_z - dz;

    // printf(" --translation--> (%f, %f, %f)", t_x, t_y, t_z);

    // rotate point relative to camera rotation
    Quat quat_point = {0, t_x, t_y, t_z};
    Quat rotated_point;
    Quat inverse_rotation;

    // printf("\ncamera rotation: (%f, %f, %f, %f)\n", rotation->a, rotation->i, rotation->j,
    // rotation->k);
    q_neg(rotation, &inverse_rotation);
    // printf("\nconjugate camera rotation: (%f, %f, %f, %f)\n", inverse_rotation.a,
    // inverse_rotation.i, inverse_rotation.j, inverse_rotation.k);

    q_mul(&rotated_point, &inverse_rotation, &quat_point);
    q_mul(&quat_point, &rotated_point, rotation);
    t_x = quat_point.i;
    t_y = quat_point.j;
    t_z = quat_point.k;

    if (t_z <= 0) {
        (*in_front) = 1;
    } else {
        (*in_front) = 0;
    }

    // printf(" --rotation--> (%f, %f, %f)\n", t_x, t_y, t_z);

    // Perspective projection
    (*out_x) = (t_x * CAMERA_DEPTH) / (t_z);
    (*out_y) = (t_y * CAMERA_DEPTH) / (t_z);
    // printf(" --perspective--> (%f, %f)\n", *out_x, *out_y);
    // printf("--------------------------------------------------------------------------\n");
}

float get_distance_to_point(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x1 - x2, dy = y1 - y2, dz = z1 - z2;
    return sqrt((dx) * (dx) + (dy) * (dy) + (dz) * (dz));
}
