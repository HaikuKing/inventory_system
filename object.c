#include "object.h"
#include <stdlib.h>

object_t *new_int(int value) {
    object_t *obj = malloc(sizeof(object_t));
    if (obj == NULL) {
        return NULL;
    }
    obj->kind = INTEGER;
    obj->data.v_int = value;
    return obj;
}

object_t *new_snek_float(float value) {
    object_t *obj = malloc(sizeof(object_t));
    if (obj == NULL) {
        return NULL;
    }
    obj->kind = FLOAT;
    obj->data.v_float = value;
    return obj;
}
