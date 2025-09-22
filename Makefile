.POSIX:

LVM_CFLAGS = ${CFLAGS} -Wall -Wextra -Wpedantic -Werror -std=c99
LVM_LDFLAGS = ${LDFLAGS}
NAME = lvm

BIN = ${NAME}
SRC = ${BIN:=.c}
OBJ = ${SRC:.c=.o}

all: ${BIN}

${BIN}: ${@:=.o}

${OBJ}:

.o:
	${CC} -o $@ $< ${LVM_LDFLAGS}

.c.o:
	${CC} -c ${LVM_CFLAGS} ${LVM_CPPFLAGS} -o $@ -c $<

clean:
	rm -f ${BIN} ${OBJ}

re: clean all

.PHONY: all clean re dist
