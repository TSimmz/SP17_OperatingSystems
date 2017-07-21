memsim				User Commands				memsim

NAME
	memsim - simulates physical memory frame assignment

SYNOPSIS
	memsim [FILE] [# FRAMES] [ALGORITHM] [MODE]

DESCRIPTION
	Takes a trace file as input and simulates the process of assigning virtual pages to frames in physical memory. It also outputs 		 		the number of simulated reads & writes to disk.

FILE
	A trace file containing addresses and a Read or Write character per each address

FRAMES
	A number of frames from 2 to 100 inclusive. If set outside this range, a default frame number of 50 will be used. This will determine the 		size of the "physical memory".

ALGORITHM
	-l, -L
		Sorts the virtual address pages into the physical memory frames using the Least Recently Used algorithm.

	-c, -C
		Sorts the virtual address pages into the physical memory frames using the Clock algorithm.

	-o, -O
		Sorts the virtual address pages into the physical memory frames using the Optimal Replacement algorithm.

	-v, -V
		Sorts the virtual address pages into the physical memory frames using the Second Chance Page Replacement algorithm.
		**NOTE: This is not currently implemented.**

MODE
	-q, -Q
		Runs the program in quiet mode. This outputs only the final count of frames, events, disk reads, and disk writes.

	-d, -D
		Runs the program in debug mode. This outputs the count of frames, events, disk reads, and disk writes during each loop of 		  	the algorithm execution. 


LEAST RECENTLY USED
	This algorithm

CLOCK
	This algorithm

OPTIMAL REPLACEMENT
	This algorithm
