#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void vms(int frames, char mode);
void lru(int frames, char mode);
void clk(int frames, char mode);
void opt(int frames, char rw, FILE *file);

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
	
//	unsigned int addr;
//	char rw;
	int count = 0;
//	int hex = 0;
//	char *address;


//*****************************************************************************
// ** readinside of each funct, files are huge, so we don't want to store them
// ** just read and process
//	while(fscanf(file,"%08x %c", &addr, &rw) != EOF){
//		//printf("Address: %08x, RW: %c\n", addr, rw);
//		count++;
//	}
//*****************************************************************************

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
			opt(frames, mode, file);
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
//**************************************************************************************
//#define NTRACES 1000000

struct trace {
	char d; // dirty or clean (R/W)
	unsigned int vpn; // virtual page number
};

bool listCheck(struct trace chk[], int vpc);
int findRM(struct trace rm[], struct trace refChk[], int c);

//OPT function
void opt(int frames, char mode, FILE *file)
{
	int ntraces = 1200000;
	struct trace refrence[ntraces]; // refrence list from all traces
	struct trace pm[frames]; // what's loaded in PM
	int hit = 0, miss = 0;
	unsigned int addr;
	char rw;

	// setting refrence and pm to zeros for empty checking
	for(int x = 0; x < sizeof(refrence); x++)
	{ refrence[x].vpn = 0; }
	for(int x = 0; x < sizeof(pm); x++)
	{ pm[x].vpn = 0; }

	// while counter
	int w = 0;
	while(fscanf(file, "%08x %c", &addr, &rw) != EOF){
		struct trace tmp = {addr, rw};
		refrence[w++] = tmp;
	}
	printf("\nfile scan complete");

	printf("\nCurrently in refrence:");
	for(int w = 0; w < sizeof(refrence); w++)
	{
		printf("\nVPN: %u, RW: %c", refrence[w].vpn, refrence[w].d);
	}

	printf("\nCurrrently in PM:");
	for(int w = 0; w < sizeof(pm); w++)
	{
		printf("\nVPN: %u, RW: %c", pm[w].vpn, pm[w].d);
	}	

	printf("\nstarting search");
	for(int i = 0; i < sizeof(refrence); i++)
	{
		if(listCheck(pm, refrence[i].vpn))
		{
			printf("\nhit");
			hit++;
			continue;
		}
		else
		{
			// getting index of what to remove from PM
			pm[findRM(pm, refrence, i)] = refrence[i];
			printf("\nmiss");
			miss++;
			continue;
		}
	}
	printf("\nRun complete:\nHits: %d\nMisses: %d\n", hit, miss);
}

// list check, checks to see if the current vpn in the refrence string is 
// already in the PM
// chk is PM
// vpc is the vpn of current spot in refrence
// returns true or false (in list or no) 
bool listCheck(struct trace chk[], int vpc)
{
	printf("\nRunning listcheck..");
	for(int j = 0; sizeof(chk); j++)
	{
		if(chk[j].vpn == vpc)
			return 1; // already in list
	}
	return 0; // not in list, find something to remove	
}

// Findind what to remove from PM to be replaced
// rm is PM and what to remove
// refChk is refrence list look ahead, see what occurs furthest away
// c is the current position in refrence string (i from opt)
// returns index of PM that is is used furthest away
int findRM(struct trace rm[], struct trace refChk[], int c)
{
	printf("\nRunning findRM..");
	int furthest = 0; // furthest used refrence from current position c
	for(int k = 0; k < sizeof(rm); k++)
	{
		if(rm[k].vpn == 0)
			return k; // PM not full, return first empty spot
		else
		{
			for(int q = ++c; k < sizeof(refChk); k++)
			{
				if(rm[k].vpn == refChk[q].vpn)
					furthest = q;
			}
		}// end else
	}// end for
	return furthest; // returning the index of the vpn that is furthest away
}
