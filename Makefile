CC=gcc
CFLAGS=-c -Wall -pthread -g -m32
TITLE=malloc_smalltest

.PHONY : all clean

# make all
all: $(TITLE) test

# make
$(TITLE): malloc.o tests/smalltest.o
	$(CC) -m32 malloc.o tests/smalltest.o -o $(TITLE)

smalltest.o: tests/smalltest.c malloc.c
	$(CC) $(CFLAGS) tests/smalltest.c malloc.c

malloc.o: malloc.c
	$(CC) $(CFLAGS) malloc.c

test:
	cd ./tests/ && $(MAKE)

# make clean
clean :
	rm -f *.o *.d $(TITLE)
	cd ./tests/ && $(MAKE) clean
 
