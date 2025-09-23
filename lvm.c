#include "lvm.h"

/*
size_t strlcpy(char *dst, const char *src, size_t siz) {
	char *d = dst;
	const char *s = src;
	size_t n = siz;
	if (n != 0) while (--n != 0) if ((*d++ = *s++) == '\0') break;
	if (n == 0) {
		if (siz != 0) *d = '\0';
		while (*s++);
	}
	return (s - src - 1);
}

char *eat(char *s, int (*p)(int), int r) {
	while (*s != '\0' && p((unsigned char)*s) == r) s++;
	return s;
}

char *token(char **ps) {
	char *s = *ps;
	char *start, *end;

	start = eat(s, isspace, 1);
	end = eat(start, isspace, 0);
	if (*end) *end++ = '\0';
	*ps = eat(end, isspace, 1);
	return start;
}

void fatal(const char *fmt, ...) {
	char buf[1024];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof buf, fmt, ap);
	va_end(ap);
	fprintf(stderr, "%s", buf);
	if(fmt[0] && fmt[strlen(fmt) - 1] == ':')
		fprintf(stderr, " %s\n", strerror(errno));
	exit(1);
}
*/

vm_p vm_new(usize heap_size) {
	vm_p vm = calloc(1, sizeof *vm);
	vm->arena = arena_new(heap_size);
	vm->halted = false;
	return vm;
}

vm_p vm_del(vm_p vm) {
	if (!vm) return NULL;
	vm->arena = arena_del(vm->arena);
	free(vm);
	return NULL;
}

u0 vm_step(vm_p vm) {
	if (is_nil(vm->prog)) { vm->halted = true; return; }
	cell_p instr = vm->prog->val.list.head;
	vm->prog = vm->prog->val.list.tail;
	if (instr && instr->tag == T_OP) {
		cell_p res = instr->val.op(vm);
		if (res) wl_push(vm, res);
	} else {
		wl_push(vm, instr);
	}
}

u0 vm_run(vm_p vm) {
	while (!vm->halted) vm_step(vm);
}

u0 vm_fatal(vm_p vm, const chr_p msg) {
	fprintf(stderr, msg);
	vm = vm_del(vm);
	exit(1);
}

// utils

u1 is_nil(cell_p x) { return x == NULL; }

usize list_len(cell_p x) {
	usize n = 0;
	for (; x && x->tag == T_LIST; x = x->val.list.tail) n++;
	return n;
}

// create cell

cell_p cell_new(vm_p vm) {
	cell_p c = arena_alloc(vm->arena, sizeof *c, alignof(cell_t));
	if (!c) vm_fatal(vm, "ERR: arena is full\n");
	return c;
}

u0 cell_print(cell_p x) {
	if (!x) { printf("nil"); return; }
	switch (x->tag) {
		case T_NUM: printf("%d", x->val.num); break;
		case T_CHR: printf("%c", x->val.chr); break;
		case T_STR: printf("\"%.*s\"", (int)x->val.str.len, 
						x->val.str.buf); break;
		case T_OP: printf("<OP>"); break;
		case T_LIST: {
			printf("(");
			cell_p it = x;
			bool first = true;
			while (it && it->tag == T_LIST) {
				if (!first) printf(" ");
				first = false;
				cell_print(it->val.list.head);
				it = it->val.list.tail;
			}
			if (it) {
				printf(" . ");
				cell_print(it);
			}
			printf(")");
		} break;
		default: printf("<unknown>"); break;
	}
}

u0 cell_println(cell_p x) {
	cell_print(x);
	printf("\n");
}

// make valuew

cell_p mk_nil(vm_p vm) {
	(u0)vm;
	return NULL;
}

cell_p mk_num(vm_p vm, i32 num) {
	cell_p c = cell_new(vm);
	c->tag = T_NUM; c->val.num = num;
	return c;
}

cell_p mk_chr(vm_p vm, chr_t chr) {
	cell_p c = cell_new(vm);
	c->tag = T_CHR; c->val.chr = chr;
	return c;
}

cell_p mk_str(vm_p vm, const chr_p str) {
	usize len = strlen(str);
	chr_p buf = arena_alloc(vm->arena, len + 1, alignof(chr_t));
	if (!buf) vm_fatal(vm, "ERR: arena is full\n");
	memcpy(buf, str, len + 1);
	cell_p c = cell_new(vm);
	c->tag = T_STR; c->val.str.buf = buf; c->val.str.len = len;
	return c;
}

cell_p mk_list(vm_p vm, cell_p head, cell_p tail) {
	cell_p c = cell_new(vm);
	c->tag = T_LIST; c->val.list.head = head; c->val.list.tail = tail;
	return c;
}

cell_p mk_op(vm_p vm, op_t f) {
	cell_p c = cell_new(vm);
	c->tag = T_OP; c->val.op = f;
	return c;
}

// working list

u0 wl_push(vm_p vm, cell_p x) { vm->wl = mk_list(vm, x, vm->wl); }

cell_p wl_pop(vm_p vm) {
	if (!vm->wl) return NULL;
	cell_p top = vm->wl->val.list.head;
	vm->wl = vm->wl->val.list.tail;
	return top;
}

cell_p wl_peek(vm_p vm) { return vm->wl ? vm->wl->val.list.head : NULL; }

// op codes

cell_p op_halt(vm_p vm) { vm->halted = true; return mk_nil(vm); }

cell_p op_link(vm_p vm) {
	cell_p tail = wl_pop(vm);
	cell_p head = wl_pop(vm);
	return mk_list(vm, head, tail);
}

cell_p op_unlink(vm_p vm) {
	cell_p p = wl_pop(vm);
	if (!p || p->tag != T_LIST) {
		wl_push(vm, mk_nil(vm));
		wl_push(vm, mk_nil(vm));
		return mk_nil(vm);
	}
	wl_push(vm, p->val.list.head);
	wl_push(vm, p->val.list.tail);
	return mk_nil(vm);
}

cell_p op_print(vm_p vm) {
	cell_p x = wl_pop(vm);
	cell_println(x);
	return mk_nil(vm);
}

// entry

cell_p list_reverse(vm_p vm, cell_p xs) {
	cell_p r = mk_nil(vm);
	while (xs) { r = mk_list(vm, xs->val.list.head, r); xs = xs->val.list.tail; }
	return r;
}

u0 prog_push(vm_p vm, cell_p *acc, cell_p x) { *acc = mk_list(vm, x, *acc); }

cell_p build_demo(vm_p vm) {
	cell_p p = mk_nil(vm);
	prog_push(vm, &p, mk_op(vm, op_halt));
	prog_push(vm, &p, mk_op(vm, op_print));
	prog_push(vm, &p, mk_op(vm, op_print));
	prog_push(vm, &p, mk_op(vm, op_print));
	prog_push(vm, &p, mk_op(vm, op_print));
	prog_push(vm, &p, mk_op(vm, op_print));
	prog_push(vm, &p, mk_op(vm, op_link));
	prog_push(vm, &p, mk_num(vm, 31));
	prog_push(vm, &p, mk_num(vm, 11));
	prog_push(vm, &p, mk_str(vm, "hello!"));
	prog_push(vm, &p, mk_str(vm, "you're beautiful!"));
	prog_push(vm, &p, mk_str(vm, "<3"));
	prog_push(vm, &p, mk_chr(vm, '!'));
	return p;
}

int main(u0) {
	vm_p vm = vm_new(1<<20);
	vm->prog = build_demo(vm);
	vm_run(vm);
	vm = vm_del(vm);
	return 0;
}
