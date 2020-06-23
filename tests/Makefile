CC=gcc
CFLAGS=-Wall -g -pthread -m32

.PHONY : all clean

SRCS=$(wildcard *.c)
EXECUTABLES=$(SRCS:.c= )
OBJ=$(SRCS:.c=.o)

all: ${EXECUTABLES}

${EXECUTABLES}: ${OBJ}
	$(CC) $(CFLAGS) ../malloc.o $@.o -o $@

${OBJ}: ${SRCS}
	$(CC) -c $(CFLAGS) $(@:.o=.c) -o $@

clean: 
	rm -f *.o ${EXECUTABLES}

