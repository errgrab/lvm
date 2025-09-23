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

vm_p	vm_new(usize heap_size);
vm_p	vm_del(vm_p vm);
u0		vm_step(vm_p vm);
u0		vm_run(vm_p vm);
u0		vm_fatal(vm_p vm, const chr_p msg);

u1		is_nil(cell_p x);
usize	list_len(cell_p x);

cell_p 	cell_new(vm_p vm);
u0		cell_print(cell_p x);
u0		cell_println(cell_p x);

cell_p	mk_nil(vm_p vm);
cell_p	mk_num(vm_p vm, i32 num);
cell_p	mk_chr(vm_p vm, chr_t chr);
cell_p	mk_str(vm_p vm, const chr_p str);
cell_p	mk_list(vm_p vm, cell_p head, cell_p tail);
cell_p	mk_op(vm_p vm, op_t f);

u0		wl_push(vm_p vm, cell_p x);
cell_p	wl_pop(vm_p vm);
cell_p	wl_peek(vm_p vm);

cell_p	op_halt(vm_p vm);
cell_p	op_link(vm_p vm);
cell_p	op_unlink(vm_p vm);
cell_p	op_print(vm_p vm);

#endif // LVM_H
