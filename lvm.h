#ifndef LVM_H
#define LVM_H

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

#include "types.h"
#include "arena.h"

#define alignof(t) offsetof(struct { char c; t x; }, x)

typedef struct vm_s	vm_t;
typedef vm_t*		vm_p;

typedef struct cell_s	cell_t;
typedef cell_t*			cell_p;

typedef enum {
	T_NIL = 0, T_LIST, T_NUM, T_CHR, T_STR, T_OP
} tag_t;

typedef cell_p (*op_t)(vm_p vm);

struct cell_s {
	tag_t tag;
	union {
		struct { cell_p head, tail; } list;
		i32 num;
		chr_t chr;
		struct { chr_p buf; usize len; } str;
		op_t op;
	} val;
};

struct vm_s {
	arena_p arena;
	cell_p wl, prog;
	u1 halted;
};

#endif // LVM_H
