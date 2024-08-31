#include "quaternions.h"

#include <math.h>

void q_add(Quat *result, Quat *q1, Quat *q2) {
    result->a += q2->a;
    result->i += q2->i;
    result->j += q2->j;
    result->k += q2->k;
}
void q_mul(Quat *result, Quat *q1, Quat *q2) {
    float a1 = q1->a, i1 = q1->i, j1 = q1->j, k1 = q1->k;
    float a2 = q2->a, i2 = q2->i, j2 = q2->j, k2 = q2->k;

    result->a = (a1 * a2) - (i1 * i2) - (j1 * j2) - (k1 * k2);
    result->i = (a1 * i2) + (i1 * a2) + (j1 * k2) - (k1 * j2);
    result->j = (a1 * j2) - (i1 * k2) + (j1 * a2) + (k1 * i2);
    result->k = (a1 * k2) + (i1 * j2) - (j1 * i2) + (k1 * a2);
}

void q_neg(Quat *q, Quat *out) {
    out->a = (q->a);
    out->i = -(q->i);
    out->j = -(q->j);
    out->k = -(q->k);
}

void euler_to_quat(Quat *quat, float angle, float x, float y, float z) {
    float theta_prime = angle / 2;
    float presin = sin(theta_prime);
    quat->a = cos(theta_prime);
    quat->i = x * presin;
    quat->j = y * presin;
    quat->k = z * presin;
}

void quat_to_euler(float *angle, float *x, float *y, float *z, Quat *quat) {
    float theta = 2 * acos(quat->a);
    if (theta == 0) {
        (*angle) = 0;
        (*x) = 0;
        (*y) = 1;
        (*z) = 0;
    } else {
        (*angle) = theta;
        (*x) = (quat->i) / (sin(theta / 2));
        (*y) = (quat->j) / (sin(theta / 2));
        (*z) = (quat->k) / (sin(theta / 2));
    }
}