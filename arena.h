#ifndef ARENA_H
#define ARENA_H

#include "types.h"

typedef struct arena_s	arena_t;
typedef arena_t*		arena_p;

struct arena_s {
	u8p		base;
	usize	cap;
	usize	off;
};

arena_p	arena_new(usize cap);
arena_p	arena_del(arena_p a);
u0p		arena_alloc(arena_p a, usize n, usize align);

#endif // ARENA_H
