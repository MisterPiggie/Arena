#define ARENA_IMPLEMENTATION
#include "arena.h"

typedef struct
{
    int x;
} foo;

int main(void)
{
    Arena a = arena_create(MB(1));
    foo *f = push_struct(&a, foo);

    return 0;
}
