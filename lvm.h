#ifndef LVM_H
#define LVM_H

typedef struct lvm_s lvm_t;
typedef struct node_s node_t;

struct node_s {
	void *val;
	node_t *next;
};

struct lvm_s {
	node_t *ip;
	node_t *mem;
	node_t *w;
};

#endif // LVM_H
