typedef enum Kind {
    INTEGER,
    FLOAT,
} object_kind_t;

typedef union Data {
    int v_int;
    float v_float;
} object_data_t;

typedef struct Object {
    object_kind_t kind;
    object_data_t data;
} object_t;

object_t *new_int(int value);
object_t *new_snek_float(float value);
