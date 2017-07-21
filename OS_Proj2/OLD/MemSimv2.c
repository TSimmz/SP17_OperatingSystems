#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
	unsigned int vpn;
	char d;
	int uBit;
}Trace;

void getEventTraceNumber(FILE *, Trace *, int *);

void vms(int, char);
void lru(Trace *, int, char);
void clk(Trace *, int, char);
void opt(Trace *, int, char);

bool pageRequestClk(Trace *, Trace, int *, int *, int *, int *, int);
bool pageRequestLru(Trace *, Trace, int *, int *, int *, int *, int);

bool listCheck(Trace chk[], int vpc, int nframes);
int findRM(Trace rm[], Trace refChk[], int c, int nframes, int traces);

int hit = 0;
int miss = 0;
int back = -1;
int front = -1;

int totalFrames = 0;		//Total Memory Frames
int eventsInTrace = 0;		//Events in Trace
int diskReads = 0;		//Total Disk Reads
int diskWrites = 0;		//Total Disk Writes


int main(int argc, char *argv[])
{	
	FILE* file;
  Trace tReference[];
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
				algo = 'l'; //default algorithm

			//argv[4] => debug|quiet
			if(argv[1] != NULL)
				mode = argv[4][0];
			else
				mode = 'q';
		}
	}

//TODO: pull file into memory here. Merge with Andrews code
	getEventTraceNumber(file, tReference, &eventsInTrace);
	
	printf("\n------File: %s, Frames: %d, Algo: %c, Mode: %c------\n", argv[1], frames, algo, mode);
	
	switch(algo){
		case 'v':
			vms(frames, mode);
			break;
		case 'l':
			lru(Trace *, frames, mode);
			break;
		case 'c':
			clk(Trace *, frames, mode);
			break;
		case 'o':
			opt(Trace *, frames, mode);
			break;
	}

	fclose(file);
	return 0;
}

void getEventTraceNumber(FILE * f, Trace * Ref, int * nTrace)
{
	unsigned int addr;	
	char rw;
  
  for(int w = 0; fscanf(file, "%08x %c", &addr, &rw) != EOF; w++){
		reference[w].vpn = addr; //converting addr to base 10 vpn
		reference[w].d = rw;
	}
  
	while(fscanf(f, "%08x %c", &addr, &rw) != EOF){		
		*nTrace++;
	}
}

//****************************************VMS function***************************************
void vms(int frames, char mode)
{
	//EXTRA CREDIT
}
//-------------------------------------------------------------------------------------------

//****************************************LRU function***************************************
void lru(FILE * f, int frames, char mode)
{
	Trace pMem[frames];	//Frame table
	Trace page; 		//Page request
	unsigned int addr;	//Address
	char rw;		//R/W
	int nTraces;

	printf("\n\tStarting LRU scan...");

	while(fscanf(f, "%08x %c", &addr, &rw) != EOF){
		fgetc(f);
		page.vpn =  addr;
		page.d = rw;
		
		nTraces++;

		if(pageRequestLru(pMem, page, &front, &back, &diskReads, &diskWrites, frames))
			hit++;
		else
			miss++;
		
		if(mode == 'd'){
			printf("\n\tTotal Memory Frames:\t%d", frames);
			printf("\n\tEvents in Trace:\t%d", nTraces);
			printf("\n\tTotal Disk Reads:\t%d", diskReads);
			printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
		}

	}
	if(mode == 'q'){
		printf("\n\tTotal Memory Frames:\t%d", frames);
		printf("\n\tEvents in Trace:\t%d", nTraces);
		printf("\n\tTotal Disk Reads:\t%d", diskReads);
		printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
	}
}
//-------------------------------------------------------------------------------------------

//****************************************CLK function***************************************
void clk(FILE* f, int frames, char mode)
{
	Trace pMem[frames];	//Frame table
	Trace page; 		//Page request
	unsigned int addr;	//Address
	char rw;		//R/W
	int nTraces;

	printf("\n\tStarting CLK scan...");

	while(fscanf(f, "%08x %c", &addr, &rw) != EOF){
		fgetc(f);
		page.vpn =  addr;
		page.d = rw;
		page.uBit = 1;
		
		nTraces++;

		if(pageRequestClk(pMem, page, &front, &back, &diskReads, &diskWrites, frames))
			hit++;
		else
			miss++;
		
		if(mode == 'd'){
			printf("\n\tTotal Memory Frames:\t%d", frames);
			printf("\n\tEvents in Trace:\t%d", nTraces);
			printf("\n\tTotal Disk Reads:\t%d", diskReads);
			printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
		}
	}

	if(mode == 'q'){
		printf("\n\tTotal Memory Frames:\t%d", frames);
		printf("\n\tEvents in Trace:\t%d", nTraces);
		printf("\n\tTotal Disk Reads:\t%d", diskReads);
		printf("\n\tTotal Disk Writes:\t%d\n\n", diskWrites);
	}
	printf("\n\tHits: %d\tMisses: %d\n", hit, miss);	
}
//-------------------------------------------------------------------------------------------

//****************************************OPT function***************************************
void opt(FILE *file, int frames, char mode)
{
  Trace pMem[frames];
  Trace reference[ntraces];
  
	int ntraces = 1000000; 
	unsigned int addr;
	char rw;

	// populating reference and pm to zeros for empty checking
	for(int x = 0; x < ntraces; x++)
	{ 
		reference[x].vpn = 0;
		reference[x].d = 'E';
	}
	for(int x = 0; x < frames; x++)
	{ 
		pm[x].vpn = 0;
		pm[x].d = 'E';
	}

	// Filescan
	for(int w = 0; fscanf(file, "%08x %c", &addr, &rw) != EOF; w++){
		reference[w].vpn = addr; //converting addr to base 10 vpn
		reference[w].d = rw;
	}
//	printf("\n***file scan complete***\n");

	// Iterating through refrence list of all traces,
	// trying to load into PM
	for(int i = 0; i < ntraces; i++)
	{
		if(!listCheck(pm, reference[i].vpn, frames))
		{		
			// findRM returns index to be replaced with current refrence vp
			int p = findRM(pm, reference, i, frames, ntraces);

			// Is the page being removed dirty?			
			if(pm[p].d == 'W') 
				diskWrites++;

			// page will be read from disk to be put in page table
			diskReads++; 			
			miss++;

			// replacement
			pm[p] = reference[i];			
			continue;
		}
		hit++;
	}
	printf("\ntotalmemory frames: %d", frames);
	printf("\nevents in trace: %d", ntraces);
	printf("\ntotal disk reads: %d", diskReads);
	printf("\ntotal disk writes: %d\n", diskWrites);
}

// list check, checks to see if the current vpn in the reference string is 
// already in the PM
// chk is PM
// vpc is the vpn of current spot in reference
// returns true or false (in list or no) 
bool listCheck(Trace chk[], int vpc, int nframes)
{
//	printf("\nRunning listcheck.");
	for(int j = 0; j < nframes; j++)
	{
		if(chk[j].vpn == vpc)
			return true; // already in list
	}
	return false; // not in list, find something to remove	
}

// Findind what to remove from PM to be replaced
// rm is PM and what to remove
// refChk is reference list look ahead, see what occurs furthest away
// c is the current position in reference string (i from opt)
// returns index of PM that is is used furthest away
int findRM(Trace rm[], Trace refChk[], int c, int nframes, int traces)
{
//	printf("\nRunning findRM");
	int furthest = 0; // furthest used reference from current position c
	int iFurthest = 0; // Index of PM to be removed
	for(int l = 0; l < nframes; l++)
	{
		if(rm[l].vpn == 0)
			return l;
	}
	for(int k = 0; k < nframes; k++)
	{
		for(int q = c + 1; q < traces; q++)
		{
			if(rm[k].vpn == refChk[q].vpn)
			{
				if(q > furthest)
				{				
					furthest = q;
					iFurthest = k;
				}				
				break;
			}
			else if(q == traces - 1) // VPN in mem does not occur again in refrence list
				return k; 
		} // end refrence for
	}// end pm for
	return iFurthest; // returning the index of the vpn that is furthest away
}

//*******************************************************************************************************
// CLK PAGE REQUEST
//*******************************************************************************************************
bool pageRequestClk(Trace * Q, Trace aTrace, int * clock, int * back, int * read, int * write, int size)
{
	int i,j,k=0;

	//**********************************
	// Queue is full
	//**********************************
	if(*clock == (*back + 1) % size)
	{
//		printf("\n\t\tQueue is full: Checking values...");
		for(i = *clock; k < size; i = (i+1)%size, k++) 			//Look through all frames
		{	
			//**********************************
			// VPN is found in [clock:back]
			//**********************************
			if(Q[i].vpn == aTrace.vpn) 				//If vpn is equal to a request vpn
			{	
				Q[i].uBit = 1;
				
//				printf("\n\t\t\tH: Address: %08x - Use = %d", Q[i].vpn, Q[i].uBit);

				return true; 					//True return => HIT!
			}

			//**********************************
			// VPN is not found
			//**********************************
			if(i == *back && Q[i].vpn != aTrace.vpn)
			{					
				for(j = *clock, k = 0; k < size; j = (j+1)%size, k++)
				{	
//					printf("\n\t\t\tH: Address: %08x\tRW: %c\tUse = %d", Q[j].vpn, Q[j].d, Q[j].uBit);
					if(Q[j].uBit == 0)
					{	
//						printf("\n\t\t\tNOT FOUND: USE = 0: REPLACING: RW: %c", Q[j].d);
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
//						printf("\nElse and 0");		//Sets each non-zero to zero
						Q[j].uBit = 0;
					}

					if(j == *back && Q[j].uBit == 0)	//If at the back and use == 1 
					{
//						printf("\nBack and 1\n");
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


//*******************************************************************************************************
// LRU PAGE REQUEST
//*******************************************************************************************************
bool pageRequestLru(Trace * Q, Trace aTrace, int * front, int * back, int * read, int * write, int size)
{
	int i,j;

	//**********************************
	// Queue is full
	//**********************************
	if(*front == (*back + 1) % size)
	{
//---		printf("\n\t\tQueue is full: Checking values...");
		for(i = *front; i != *back; i = (i+1)%size) 			//Look through all frames
		{	
			
			
			//**********************************
			// VPN is found at the front
			//**********************************
			if(i == *front && Q[i].vpn == aTrace.vpn)		//If evictee is at the back already
			{	
//---				printf("\n\t\t\tH: Address: %08x - Evicted", Q[i].vpn);
				
				*front = (*front + 1) % size;			//Increment front
				*back = (*back + 1) % size;			//Increment back

				return true;					//True return => HIT!
			}
			//**********************************
			// VPN is found in [front+1:back-1]
			//**********************************
			if(Q[i].vpn == aTrace.vpn) 				//If vpn is equal to a request vpn
			{
//---				printf("\n\t\t\tValue found @ %d: shifting---", i);

				for(j = i;j < *back; j = (j+1)%size)		//Loop shifts everything down
				{
					Q[j] = Q[(j+1)%size];
				}
				Q[*back] = aTrace;				//Sets aTrace to the end of queue
//---				printf("\n\t\t\tH: Address: %08x - Evicted", aTrace.vpn);
				return true; 					//True return => HIT!
			}
			//**********************************
			// VPN is found at back
			//**********************************
			int j = (i+1)%size;
			if(j == *back && Q[j].vpn == aTrace.vpn)		//If evictee is at the back already
			{	
//---				printf("\n\t\t\tM: Address: %08x - Evicted", Q[j].vpn);
				
				Q[*back] = aTrace;
				return true;					//True return => HIT!
			}	
			//**********************************
			// VPN is not found
			//**********************************
			if(j == *back && Q[j].vpn != aTrace.vpn)		//If evictee isnt at the back
			{	
//---				printf("\n\t\t\tM: Address: %08x - Evicted", Q[j].vpn);
				
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
		{
//---			printf("\n\t\tQueue is empty: initializing...");
										//Initializes queue of structs to zero
			*front = 0;
		
			for(i = 0; i < size; i++)
			{	
				Q[i].vpn = 0;
				Q[i].d = 0; 
			}
		}	
	}

//---	printf("\n\t\t\tM: Adding %08x to end...", aTrace.vpn);
	*back = (*back + 1) % size;
	Q[*back] = aTrace;
	
	*read = *read + 1;
	return false;								//False return => MISS!
}
