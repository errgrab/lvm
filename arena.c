#include "arena.h"

#include <stdlib.h>

arena_p arena_new(usize cap) {
	arena_p a = calloc(1, sizeof *a);
	a->base = calloc(1, cap);
	a->cap = cap;
	a->off = 0;
	return a;
}

arena_p arena_del(arena_p a) {
	if (!a) return NULL;
	free(a->base);
	free(a);
	return NULL;
}

u0p arena_alloc(arena_p a, usize n, usize align) {
	usize p = (a->off + (align - 1)) & ~(align - 1);
	if (p + n > a->cap) return NULL;
	u0p ptr = a->base + p;
	a->off = p + n;
	return ptr;
}
