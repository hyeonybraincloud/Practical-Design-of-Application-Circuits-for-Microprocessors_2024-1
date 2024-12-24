//-------| lib/main.h |-------//
#ifndef	MAIN_H
#define	MAIN_H 

#define START_SCORE 5

#include <stdio.h>		// printf(), scanf(), fprintf()
#include <fcntl.h>		// open()
#include <unistd.h>		// close(), off_t , usleep()
#include <sys/mman.h>		// mmap(), munmap()
#include <stdlib.h> 		// exit()
#include <string.h>		// strlen()


#include "ieb.h"
#include "led.h"
#include "dot.h"
#include "fnd.h"
#include "clcd.h"
#include "keypad.h"

int KEYPAD[MAX_KEY_ROW][MAX_KEY_COL] = { 0 };

typedef struct Player {
	int score;
	char name[20];
}Player;

typedef enum {
	FALSE = 0,
	TRUE  = 1
} truth_t;

typedef enum {
	ERROR	= -1,
	SUCCESS	= 0
} error_t;

int main(int argc, char* argv[]);
short * mapper(off_t offset, int prot);
void unmapper();
void emergency_closer();

void print_clcd(int option);
truth_t logic(int p, int* wrong_key_cnt);
void input_mode(int p, int** wrong_key_cnt);
void show_score();
void compare_data(int p);
void clear_KEYPAD_Arr();
int PRESS(int p);
void final_mode();
int play_mode();

#endif
