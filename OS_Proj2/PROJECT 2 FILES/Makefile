all:	memsim
memsim:	memsim.o
	gcc -std=c99 -o memsim memsim.o
memsim.o: memsim.c
	gcc -std=c99 -c memsim.c
clean:
	rm memsim *.o

