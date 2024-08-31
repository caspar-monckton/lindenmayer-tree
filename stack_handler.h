#ifndef STACK_HANDLER_C_
#define STACK_HANDLER_C_

#include "shared.h"

void init_float_stack(FloatStack *stack, int capacity);

float pop_float_stack(FloatStack *stack);

void push_float_stack(FloatStack *stack, float source);

void free_float_stack(FloatStack *stack);

#endif