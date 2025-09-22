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
	${CC} -o $@ $< ${SIS_LDFLAGS}

.c.o:
	${CC} -c ${SIS_CFLAGS} ${SIS_CPPFLAGS} -o $@ -c $<

clean:
	rm -f ${BIN} ${OBJ}

re: clean all

.PHONY: all clean re dist
