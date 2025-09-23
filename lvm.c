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

u0 fatal_arena_full(vm_p vm) {
	fprintf(stderr, "ERR: arena full\n");
	vm->arena = arena_del(vm->arena);
	exit(1);
}

cell_p cell_new(vm_p vm) {
	cell_p c = arena_alloc(vm->arena, sizeof *c, alignof(cell_t));
	if (!c) fatal_arena_full(vm);
	return c;
}

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

cell_p mk_str(vm_p vm, const chr_p s) {
	usize len = strlen(s);
	chr_p buf = arena_alloc(vm->arena, len + 1, alignof(chr_t));
	if (!buf) fatal_arena_full(vm);
	memcpy(buf, s, len + 1);
	cell_p c = cell_new(vm);
	c->tag = T_STR; c->val.str.buf = buf; c->val.str.len = len;
	return c;
}

cell_p mk_list(vm_p vm, cell_p head, cell_p tail) {
	cell_p c = cell_new(vm);
	c->tag = T_LIST; c->val.list.head = head; c->val.list.tail = tail;
	return c;
}

int main(u0) {
	return 0;
}
