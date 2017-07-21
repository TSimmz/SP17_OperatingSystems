#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Struct to function as trace "object"
typedef struct{
	unsigned int vpn;
	char d;
	int uBit;
}Trace;

bool getEventTraces(FILE *, Trace *);

//Algorithm definitions
void vms(int, char);
void lru(Trace *, int, char);
void clk(Trace *, int, char);
void opt(Trace *, int, char);

//Utilty functions to be used by the algorithm functions
bool pageRequestClk(Trace *, Trace, int *, int *, int *, int *, int);
bool pageRequestLru(Trace *, Trace, int *, int *, int *, int *, int);
bool refCheck(Trace *, Trace, int);
int findRM(Trace *, Trace *, int, int);

//Global variables
int hit = 0;
int miss = 0;
int back = -1;
int front = -1;

int totalFrames = 0;		//Total Memory Frames
int eventsInTrace = 1000000;	//Events in Trace
int diskReads = 0;		//Total Disk Reads
int diskWrites = 0;		//Total Disk Writes


int main(int argc, char *argv[])
{	
	FILE * file;
  	Trace tReference[eventsInTrace];
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
		if(!getEventTraces(fopen(argv[1], "r"), tReference))
		{
			printf("File could not be opened.");
			return 1;
		}else{	
			//argv[2] => nframes 
			if(argv[2] != NULL)
			{
				frames = atoi(argv[2]);
				if(frames < 2 || frames > 100)
					frames = 50; 		//default frames
			} 

			//argv[3] => algorithm to use (vms|lru|clk|opt)
			if(argv[3] != NULL)
				algo = argv[3][1];
			else
				algo = 'l'; //default algorithm

			//argv[4] => debug|quiet
			if(argv[1] != NULL)
				mode = argv[4][1];
			else
				mode = 'q'; //default mode
		}
	}
	
	printf("\n------File: %s, Frames: %d, Algo: %c, Mode: %c------\n", argv[1], frames, algo, mode);
	
	switch(algo){
		case 'v':
		case 'V':
			vms(frames, mode);
			break;
		case 'l':
		case 'L':
			lru(tReference, frames, mode);
			break;
		case 'c':
		case 'C':
			clk(tReference, frames, mode);
			break;
		case 'o':
		case 'O':
			opt(tReference, frames, mode);
			break;
	}
	return 0;
}

//*******************************************************************************************
// bool getEventTraces
// FILE * file	- the file, as input from the user
// Trace * Ref	- Reference array of structs containing the entire trace file
//
// Takes in the file and parses it into an array of Trace structs
//*******************************************************************************************
bool getEventTraces(FILE * file, Trace * Ref)
{
	if(file == 0)
		return false;	
	
	unsigned int addr;	
	char rw;
	int i;
	  
	for(i = 0; fscanf(file, "%08x %c", &addr, &rw) != EOF; i++)
	{	
		fgetc(file);
		Ref[i].vpn = addr/4096; 	//Grabs the first bits - virtual page # 
		Ref[i].d = rw;			//Grabs the R/W char
		Ref[i].uBit = 1;		//Sets the UseBit to 1
	}	
	fclose(file);
	return true;
}

//****************************************VMS function***************************************
void vms(int frames, char mode)
{
	//EXTRA CREDIT
	printf("VMS has not been implmemented - Try another algorithm.");
}

//*******************************************************************************************
// void clk()
// Trace * Ref	- Reference array of structs containing the entire trace file
// int frames	- the number of frames, as entered by the user
// char mode 	- the mode, as input by the user
//
// Takes in the reference array, number of frames, and mode and uses the CLK algorithm
// 	to, ultimately, count the number of reads and writes to disk the trace makes.
//*******************************************************************************************
void lru(Trace * Ref, int frames, char mode)
{
	Trace pMem[frames];	//Frame table
	int i, k = 0;

	printf("\n\tStarting LRU algorithm...\n");

	for(i = 0; i < eventsInTrace; i++)
	{
		if(pageRequestLru(pMem, Ref[i], &front, &back, &diskReads, &diskWrites, frames))
			hit++;
		else
			miss++;
		
		if((mode == 'd' || mode == 'D')){
			printf("\n\n\tTotal Memory Frames:\t%d", frames);
			printf("\n\tEvents in Trace:\t%d", eventsInTrace);
			printf("\n\tTotal Disk Reads:\t%d", diskReads);
			printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
		}
		if((mode == 'q' || mode == 'Q') && i%100000 == 0)
			printf("\n\t\tProgress: %d%\t", k += 10);
	}
	if((mode == 'q' || mode == 'Q')){
		printf("\n\n\tTotal Memory Frames:\t%d", frames);
		printf("\n\tEvents in Trace:\t%d", eventsInTrace);
		printf("\n\tTotal Disk Reads:\t%d", diskReads);
		printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
	}
	printf("\t...LRU algorithm complete\n\n");
}

//*******************************************************************************************
// void lru()
// Trace * Ref	- Reference array of structs containing the entire trace file
// int frames	- the number of frames, as entered by the user
// char mode 	- the mode, as input by the user
//
// Takes in the reference array, number of frames, and mode and uses the LRU algorithm
// 	to, ultimately, count the number of reads and writes to disk the trace makes.
//*******************************************************************************************
void clk(Trace * Ref, int frames, char mode)
{
	Trace pMem[frames];	//Frame table
	int i, k = 0;

	printf("\n\tStarting CLK algorithm...\n");

	for(i = 0; i < eventsInTrace; i++)
	{
		if(pageRequestClk(pMem, Ref[i], &front, &back, &diskReads, &diskWrites, frames))
			hit++;
		else
			miss++;
		
		if((mode == 'd' || mode == 'D')){
			printf("\n\n\tTotal Memory Frames:\t%d", frames);
			printf("\n\tEvents in Trace:\t%d", eventsInTrace);
			printf("\n\tTotal Disk Reads:\t%d", diskReads);
			printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
		}
		if((mode == 'q' || mode == 'Q') && i%100000 == 0)
			printf("\n\t\tProgress: %d%\t", k += 10);
	}

	if((mode == 'q' || mode == 'Q')){
		printf("\n\n\tTotal Memory Frames:\t%d", frames);
		printf("\n\tEvents in Trace:\t%d", eventsInTrace);
		printf("\n\tTotal Disk Reads:\t%d", diskReads);
		printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
	}	
	printf("\t...CLK algorithm complete\n\n");
}

//*******************************************************************************************
// void opt()
// Trace * Ref	- Reference array of structs containing the entire trace file
// int frames	- the number of frames, as entered by the user
// char mode 	- the mode, as input by the user
//
// Takes in the reference array, number of frames, and mode and uses the OPT algorithm
// 	to, ultimately, count the number of reads and writes to disk the trace makes.
//*******************************************************************************************
void opt(Trace * Ref, int frames, char mode)
{
	Trace pMem[frames];
	int i, j, k = 0;

	printf("\n\tStarting OPT algorithm...\n");	
	
	for(i = 0; i < frames; i++)
	{ 
		pMem[i].vpn = 0;
		pMem[i].d = 0;
	}

	for(i = 0; i < eventsInTrace; i++)						// Iterating through refrence list of all traces
	{	
		if(!refCheck(pMem, Ref[i], frames))
		{		
			j = findRM(pMem, Ref, i, frames);

			if(pMem[j].d == 'W') 						// Is the page being removed dirty?
				diskWrites++;

			diskReads++;							// page will be read from disk to be put in page table
			miss++;

			pMem[j] = Ref[i];						// replacement	
			continue;
		}
		hit++;
		
		if((mode == 'd' || mode == 'D')){
			printf("\n\n\tTotal Memory Frames:\t%d", frames);
			printf("\n\tEvents in Trace:\t%d", eventsInTrace);
			printf("\n\tTotal Disk Reads:\t%d", diskReads);
			printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
		}
		if((mode == 'q' || mode == 'Q') && i%100000 == 0)
			printf("\n\t\tProgress: %d%\t", k += 10);
	}

	if((mode == 'q' || mode == 'Q')){
		printf("\n\n\tTotal Memory Frames:\t%d", frames);
		printf("\n\tEvents in Trace:\t%d", eventsInTrace);
		printf("\n\tTotal Disk Reads:\t%d", diskReads);
		printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
	}
	printf("\t...OPT algorithm complete\n\n");
}

//*******************************************************************************************
// bool refCheck() 
// Trace *pMem	- Array of Trace structs that represents the physical memory of the system
// Trace Ref	- The current trace in the array of trace structs to check
// 
// Checks to see if the current vpn in the reference trace struct is already in the PM 
//*******************************************************************************************
bool refCheck(Trace *pMem, Trace Ref, int frames)
{
//	printf("\nRunning listcheck.");
	for(int j = 0; j < frames; j++)
	{
		if(pMem[j].vpn == Ref.vpn)			// already in list
			return true; 							
	}
	return false; 						// not in list, find something to remove	
}

//*******************************************************************************************
// int findRM() 
// Trace *pMem	- Array of Trace structs that represents the physical memory of the system
// Trace Ref	- The current trace in the array of trace structs to check
// int curr 	- The current position in reference array of trace structs
// int size 	- The number of frames, as entered by the user
// 
// Finding what to be evicted from PM and be replaced
//	Returns the index the trace in PM that is farthest away in the reference array
//*******************************************************************************************
int findRM(Trace * pMem, Trace Ref[], int curr, int size)
{
	int furthest = 0; 					// furthest used reference from current position c
	int furthFrame = 0; 					// Index of PM to be removed
	int i, q;
	
	for(i = 0; i < size; i++)
	{
		if(pMem[i].vpn == 0)
			return i;
	}
	
	for(i = 0; i < size; i++)
	{
		for(q = curr+1; q < eventsInTrace; q++)
		{
			if(pMem[i].vpn == Ref[q].vpn)
			{
				if(q > furthest)
				{				
					furthest = q;
					furthFrame = i;
				}				
				break;
			}
			else if(q == eventsInTrace - 1) 	// VPN in mem does not occur again in refrence list
				return i;
		} 
	}
	return furthFrame; 					// returning the index of the vpn that is furthest away
}

//*******************************************************************************************
// bool pageRequestClk()
// Trace * Q	- Array of Trace structs that represents the physical memory of the system
// Trace * Ref	- Reference array of structs containing the entire trace file
// int *clock 	- The clock hand as it moves around the circular array
// int *back 	- The back of the circular array
// int *read	- Holds the read counter
// int *write	- Holds the write counter
// int size	- The number of frames, as entered by the user
//
// This handles the actual algorithm arithmetic part of the CLK frame assignment algorithm
//*******************************************************************************************
bool pageRequestClk(Trace * Q, Trace aTrace, int * clock, int * back, int * read, int * write, int size)
{
	int i,j,k=0;

	//**********************************
	// Queue is full
	//**********************************
	if(*clock == (*back + 1) % size)
	{
		for(i = *clock; k < size; i = (i+1)%size, k++) 			//Look through all frames
		{	
			//**********************************
			// VPN is found in [clock:back]
			//**********************************
			if(Q[i].vpn == aTrace.vpn) 				//If vpn is equal to a request vpn
			{	
				Q[i].uBit = 1;
				return true; 					//True return => HIT!
			}

			//**********************************
			// VPN is not found
			//**********************************
			if(i == *back && Q[i].vpn != aTrace.vpn)
			{					
				for(j = *clock, k = 0; k < size; j = (j+1)%size, k++)
				{	
					if(Q[j].uBit == 0)
					{	
						if(Q[j].d == 'W'){		//Counts Read/Writes
							*write = *write + 1;					
						}

						Q[j] = aTrace;
					 	*clock = (j + 1) % size;
						*back = j;
						
						*read = *read + 1;
						return false;			//False return => MISS!
					}
					
					if(Q[j].uBit != 0)
					{		
						Q[j].uBit = 0;			//Sets each non-zero to zero
					}

					if(j == *back && Q[j].uBit == 0)	//If at the back and use == 1 
					{
						Q[*clock] = aTrace;
						return false;			//False return => MISS!		
					} 
				} 					
			}						
		}
	}
	else{
		//**********************************
		// Queue is empty 
		//**********************************	
		if(*back == -1 && *clock == -1)					//Initializes queue of structs to zero
		{			
			*back = 0;
		
			for(i = 0; i < size; i++)
			{	
				Q[i].vpn = 0;
				Q[i].d = 0;
				Q[i].uBit = 0;
			}
		}	
	}

	if(*clock == -1 && *back == 0){						//First iteration only		
		*clock = 0;
		Q[*clock] = aTrace;
		*read = *read + 1;
		return false;

	}
			
	*back = (*back + 1) % size;
	Q[*back] = aTrace;
	*read = *read + 1;
	return false;								//False return => MISS!
}


//*******************************************************************************************
// bool pageRequestLru()
// Trace * Q	- Array of Trace structs that represents the physical memory of the system
// Trace * Ref	- Reference array of structs containing the entire trace file
// int *front 	- The front of the circular array
// int *back 	- The back of the circular array
// int *read	- Holds the read counter
// int *write	- Holds the write counter
// int size	- The number of frames, as entered by the user
//
// This handles the actual algorithm arithmetic part of the LRU frame assignment algorithm
//*******************************************************************************************
bool pageRequestLru(Trace * Q, Trace aTrace, int * front, int * back, int * read, int * write, int size)
{
	int i,j;

	//**********************************
	// Queue is full
	//**********************************
	if(*front == (*back + 1) % size)
	{
		for(i = *front; i != *back; i = (i+1)%size) 			//Look through all frames
		{	
			
			
			//**********************************
			// VPN is found at the front
			//**********************************
			if(i == *front && Q[i].vpn == aTrace.vpn)		//If evictee is at the back already
			{	
				*front = (*front + 1) % size;			//Increment front
				*back = (*back + 1) % size;			//Increment back

				return true;					//True return => HIT!
			}
			//**********************************
			// VPN is found in [front+1:back-1]
			//**********************************
			if(Q[i].vpn == aTrace.vpn) 				//If vpn is equal to a request vpn
			{
				for(j = i;j < *back; j = (j+1)%size)		//Loop shifts everything down
				{
					Q[j] = Q[(j+1)%size];
				}
				Q[*back] = aTrace;				//Sets aTrace to the end of queue
				return true; 					//True return => HIT!
			}
			//**********************************
			// VPN is found at back
			//**********************************
			int j = (i+1)%size;
			if(j == *back && Q[j].vpn == aTrace.vpn)		//If evictee is at the back already
			{	
				Q[*back] = aTrace;
				return true;					//True return => HIT!
			}	
			//**********************************
			// VPN is not found
			//**********************************
			if(j == *back && Q[j].vpn != aTrace.vpn)		//If evictee isnt at the back
			{				
				if(Q[j].d == 'W'){				//Counts Read/Writes
					*write = *write + 1;					
				}
				*read = *read + 1;				

				*front = (*front + 1) % size;			//Increment front
				*back = (*back + 1) % size;			//Increment back

				Q[*back] = aTrace;				//Assign to new last
				return false;					//False return => MISS!
			}						
		}
	}
	else{
		//**********************************
		// Queue is empty 
		//**********************************
		if(*back == -1 && *front == -1)
		{								//Initializes queue of structs to zero
			*front = 0;

			for(i = 0; i < size; i++)
			{	
				Q[i].vpn = 0;
				Q[i].d = 0; 
			}
		}	
	}

	*back = (*back + 1) % size;
	Q[*back] = aTrace;
	
	*read = *read + 1;
	return false;								//False return => MISS!
}
