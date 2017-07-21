#include <stdio.h>
#include <stdlib.h>

void vms(int frames, char mode);
void lru(int frames, char mode);
void clk(int frames, char mode);
void opt(int frames, char mode);

int totFrms;
int evntNum;
int diskReads;
int diskWrites;

int main(int argc, char *argv[])
{	
	FILE *file;
	int frames;
	char algo;
	char mode;

	if (argc != 5)
	{
		printf("Usage: %s filename", argv[0]);
	}
	else
	{
		//argv[1] => tracefile
		file = fopen(argv[1], "r");
		
		if(file == 0)
			printf("File could not be opened.");
		else{	
			//argv[2] => nframes 
			if(argv[2] != NULL)
				frames = atoi(argv[2]);
			else
				frames = 500; //default frames 
		
			//argv[3] => algorithm to use (vms|lru|clk|opt)
			if(argv[3] != NULL)
				algo = argv[3][0];
			else
				algo = 'v'; //default algorithm

			//argv[4] => debug|quiet
			if(argv[1] != NULL)
				mode = argv[4][0];
			else
				mode = 'q';
		}
	}
	
	printf("File: %s, Frames: %d, Algo: %c, Mode: %c\n", argv[1], frames, algo, mode);
	
	unsigned int addr;
	char rw;
	int count = 0;
	int hex = 0;
	char *address;

	while(fscanf(file,"%08x %c", &addr, &rw) != EOF){
		//printf("Address: %08x, RW: %c\n", addr, rw);
		count++;
	}
	printf("Addresses: %d\n", count);
	switch(algo){
		case 'v':
			vms(frames, mode);
			break;
		case 'l':
			lru(frames, mode);
			break;
		case 'c':
			clk(frames, mode);
			break;
		case 'o':
			opt(frames, mode);
			break;
	}
}
//PageNumber function

//VMS function
void vms(int frames, char mode){}

//LRU function
void lru(int frames, char mode){}

//CLK function
void clk(int frames, char mode){}

//OPT function
void opt(int frames, char mode){}


