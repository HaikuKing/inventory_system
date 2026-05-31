#include "stack.h"
#include <stdlib.h>
#include <stddef.h>

stack_t *stack_new(size_t capacity) {
    stack_t *stackptr = malloc(sizeof(stack_t));
    if (stackptr == NULL) {
        return NULL;
    }
    stackptr->count = 0;
    stackptr->capacity = capacity;
    stackptr->data = malloc((capacity * sizeof(void *)));

    if (stackptr->data == NULL) {
        free(stackptr);
        return NULL;
    }
    return stackptr;
}

void stack_push(stack_t *stack, void *obj) {
    if (stack->count == stack->capacity) {
        stack->capacity *= 2;
        void **new_data = realloc(stack->data, stack->capacity * sizeof(void *));
        if (new_data == NULL) {
            stack->capacity /= 2;
            return;
        }
        stack->data = new_data;
    }
    stack->data[stack->count] = obj;
    stack->count++;
    return;
}

void *stack_pop(stack_t *stack) {
    if (stack->count == 0) {
        return NULL;
    }
    stack->count -= 1;
    return stack->data[stack->count];
}

void stack_free(stack_t *stack) {
    if (stack == NULL) {
        return;
    }
    if (stack->data != NULL) {
        free(stack->data);
    }
    free(stack);
}
