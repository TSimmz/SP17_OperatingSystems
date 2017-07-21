#ifndef _CUBE_H
#define _CUBE_H

#define SOLUTION

struct cube;

int total; 
sem_t wiz, user;


struct wizard {
  int x;
  int y;
  int id;
  char team;
  int status;	/* 0: alive, 1: frozen */
  struct cube *cube;
  pthread_t pTid;
  sem_t frz;	/* Semaphore to sleep frozen wizard */
};
  
struct room {
  int x;
  int y;
  struct wizard *wizards[2];
  int status;	/* Stores value of sem_val */
  sem_t rm;		/* Semaphore to keep room count */
};

struct cube {
  int size;
  int teamA_size;
  int teamB_size;
  struct wizard **teamA_wizards;
  struct wizard **teamB_wizards;
  int game_status;

  /* Pointer to a two-dimensional array of rooms */
  struct room ***rooms;
};

extern void print_wizard(struct wizard *);	
extern void kill_wizards(struct cube *, pthread_t *);
extern void print_cube(struct cube *);		// Shows entire cube
extern bool check_winner(struct cube *);	// Check to see if a team won

extern void dostuff();
extern struct room * choose_room(struct wizard*);	//Wizard chooses a room to try
extern int try_room(struct wizard *, struct room *, struct room* );	//Wizard tries room to switch
extern void switch_rooms(struct wizard *, struct room *, struct room* );	//Wizard switches room
extern struct wizard * find_opponent(struct wizard*, struct room *);	//Wizard searches for opponent
extern int fight_wizard(struct wizard *, struct wizard *, struct room *);	//Wizard freeze spell
extern int free_wizard(struct wizard *, struct wizard *, struct room *);	//Wizard unfreeze spell

#endif
