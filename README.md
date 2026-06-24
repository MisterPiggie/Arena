# STB-styled Arena implementation

A lightweight single-header arena allocator for C that uses virtual memory reservation and on-demand page commitment for fast allocations with minimal overhead.
Linux only.

## Installation

Include the header in your project:

```c
#include "arena.h"
```

In exactly one source file:

```c
#define ARENA_IMPLEMENTATION
#include "arena.h"
```

## Quick Start

```c
#define ARENA_IMPLEMENTATION
#include "stb_arena.h"

int main(void)
{
    Arena arena = arena_create(GB(1), MB(4));

    int *numbers = arena_push_array(&arena, int, 1000);

    for (int i = 0; i < 1000; i++)
        numbers[i] = i;

    arena_destroy(&arena);
    return 0;
}
```

## Creating an Arena

```c
Arena arena = arena_create(GB(1), MB(4));
```

The allocator reserves a large virtual address range and commits memory in fixed-size chunks as allocations grow.
Memory is reserved up front using `mmap()` and committed incrementally with `mprotect()`. This allows large arenas to be created without immediately consuming physical memory.

## Allocation

Allocate arbitrary memory:

```c
void *ptr = arena_push(&arena, 256);
```

Allocate and zero memory:

```c
void *ptr = arena_push_zero(&arena, 256);
```

Allocate arrays:

```c
float *values = arena_push_array(&arena, float, 1024);
```

Allocate structs:

```c
object *obj = arena_push_struct(&arena, MyStruct);
```

Zero-initialized variants:

```c
object *obj = arena_push_struct_zero(&arena, MyStruct);

float *values = arena_push_array_zero(&arena, float, 1024);
```

## String Helpers

Copy a null-terminated string:

```c
char *name = arena_push_str(&arena, "Hello World");
```

Copy the first N characters:

```c
char *name = arena_push_strn(&arena, source, length);
```

Formatted strings:

```c
char *message = arena_push_strf(&arena, "Player %d scored %d points", player_id, score);
```

## Rewind vs Reset

### Rewind

```c
arena_rewind(&arena);
```

Resets allocation position to the beginning while keeping committed memory available.

Useful for frame allocators and temporary scratch memory.

### Reset

```c
arena_reset(&arena);
```

Returns committed pages back to the operating system and resets the arena completely.

Use when memory usage should be released.

## Destruction

```c
arena_destroy(&arena);
```

Releases the entire reserved virtual memory region.


