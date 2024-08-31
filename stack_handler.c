#include "stack_handler.h"

void init_float_stack(FloatStack *stack, int capacity) {
    stack->capacity = capacity;
    stack->top = -1;
    stack->items = malloc(capacity * sizeof(float));
}

float pop_float_stack(FloatStack *stack) {
    if (stack->top == -1) {
        printf("Stack underflow.\n");
        return -42069;
    } else {
        --(stack->top);
        return (stack->items)[(stack->top) + 1];
    }
}

void push_float_stack(FloatStack *stack, float source) {
    if (stack->top + 1 == stack->capacity) {
        printf("Stack overflow.\n");
    } else {
        (stack->items)[++(stack->top)] = source;
    }
}

void free_float_stack(FloatStack *stack) {
    free(stack->items);
}