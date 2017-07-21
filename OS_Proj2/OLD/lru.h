#ifndef LRU_H_
#define LRU_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct{
	unsigned int vpn;
	char d;
	int uBit;
}Trace;

bool pageRequest(Trace *, Trace, int *, int *, int *, int *, int);

bool pageRequest(Trace * Q, Trace aTrace, int * front, int * back, int * read, int * write, int size)
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
			if(j == *back && Q[j].vpn != aTrace.vpn)		//If evictee is at the back already
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

#endif
