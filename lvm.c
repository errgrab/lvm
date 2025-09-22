#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lvm.h"

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
