CC=gcc
CFLAGS=-I.
DEPS=wizard.h cube.h

%.o: %.c $(DEPS) ; $(CC) -c -o $@ $< $(CFLAGS)

cube: cube.o ; gcc -o cube cube.o  -lreadline -lncurses -lpthread -I.

.PHONY : clean

clean: ; rm -f *.o cube
