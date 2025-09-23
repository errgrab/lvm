.POSIX:

LVM_CFLAGS = ${CFLAGS} -Wall -Wextra -Wpedantic -Werror -std=c99
LVM_LDFLAGS = ${LDFLAGS}
NAME = lvm

SRC = lvm.c arena.c
OBJ = ${SRC:.c=.o}

all: ${NAME}

${NAME}: ${OBJ}
	${CC} ${OBJ} -o $@ ${LVM_LDFLAGS}

.c.o:
	${CC} ${LVM_CFLAGS} -o $@ -c $<

clean:
	rm -f ${NAME} ${OBJ}

re: clean all

.PHONY: all clean re
