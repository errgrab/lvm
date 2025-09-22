#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lvm.h"

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

lvm_t *get_lvm() {
	static lvm_t lvm = {0};
	return &lvm;
}

node_t *new_node(void *val) {
	node_t *n = calloc(1, sizeof *n);
	n->val = val;
	return n;
}

void *del_node(node_t *n) {
	while (n) {
		node_t *next = n->next;
		free(n);
		n = next;
	}
	return NULL;
}

node_t *append(node_t *dst, node_t *src) {
	node_t *d = dst;
	while (d->next) d = d->next;
	d->next = src;
	return dst;
}

node_t *prepend(node_t *dst, node_t *src) {
	src->next = dst;
	return src;
}

int main(void) {
	return 0;
}
