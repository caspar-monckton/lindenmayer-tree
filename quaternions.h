#include "shared.h"

#ifndef QUATERNIONS_C_
#define QUATERNIONS_C_

/*All of these operations act on the first parameter by applying the second parameter to it. I.e. it
 * mutates q1 and preserves q2.*/
void q_add(Quat *result, Quat *q1, Quat *q2);
void q_mul(Quat *result, Quat *q1, Quat *q2);

/*All of these operations return the outcome without mutating parameter(s).*/
void q_neg(Quat *q, Quat *out);

void quat_to_euler(float *angle, float *x, float *y, float *z, Quat *quat);
void euler_to_quat(Quat *quat, float angle, float x, float y, float z);

#endif