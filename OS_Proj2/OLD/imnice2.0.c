//OPT function
void opt(int frames, int ntraces, char mode, FILE *file)
{
	int diskRead = 0, diskWrite = 0; // total reads and writes to disk

	// addr and rw used to read input
	unsigned int addr;
	char rw;
	Trace pm[frames]; // what's loaded in PM
	Trace reference[ntraces]; // reference list of all traces

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
		reference[w].vpn = addr/4096; //converting addr to base 10 vpn
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
				diskWrite++;

			// page will be read from disk to be put in page table
			diskRead++; 			
			miss++;

			// replacement
			pm[p] = reference[i];			
			continue;
		}
		hit++;
	}
	printf("\ntotalmemory frames: %d", frames);
	printf("\nevents in trace: %d", ntraces);
	printf("\ntotal disk reads: %d", diskRead);
	printf("\ntotal disk writes: %d\n", diskWrite);
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
