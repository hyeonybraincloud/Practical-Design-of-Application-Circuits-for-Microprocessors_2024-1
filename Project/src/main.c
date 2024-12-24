//-------| src/main.c |-------//
#include "main.h"

static off_t IEB_DOT[MAX_DOT] = {
	IEB_DOT1,
	IEB_DOT2,
	IEB_DOT3,
	IEB_DOT4,
	IEB_DOT5
};
static off_t IEB_FND[MAX_FND] = {
	IEB_FND0,
	IEB_FND1,
	IEB_FND2,
	IEB_FND3,
	IEB_FND4,
	IEB_FND5,
	IEB_FND6,
	IEB_FND7
};

static int fd;
static int map_counter = 0;
static void * map_data[100];

// Player 1, 2, 3, 4
Player P1, P2, P3, P4;

int main(int argc, char* argv[]) {

/*------Start Initial Setup------*/
	int i;
	short * led, * dot[MAX_DOT], * fnd[MAX_FND];
	short * clcd_cmd, * clcd_data, * keypad_out, * keypad_in;
	
	fd = open("/dev/mem", O_RDWR|O_SYNC);
	if (fd == -1) {
		fprintf(stderr, "Cannot open /dev/mem file");
		exit(EXIT_FAILURE);
	}
	
	led = mapper(IEB_LED, PROT_WRITE);
	for( i=0; i<MAX_DOT; i++ ) {
		dot[i] = mapper(IEB_DOT[i], PROT_WRITE);
	}
	for( i=0; i<MAX_FND; i++ ) {
		fnd[i] = mapper(IEB_FND[i], PROT_WRITE);
	}
	clcd_cmd  = mapper(IEB_CLCD_CMD, PROT_WRITE);
	clcd_data = mapper(IEB_CLCD_DATA, PROT_WRITE);
	keypad_out  = mapper(IEB_KEY_W, PROT_WRITE);
	keypad_in = mapper(IEB_KEY_R, PROT_READ);
	
	init_led(led);
	init_dot(dot);
	init_fnd(fnd);
	init_clcd(clcd_cmd, clcd_data);
	init_keypad(keypad_out, keypad_in);

/*------Finish Initial Setup------*/
	if (argc != 5)
		print_clcd(0);

/*------Start Player Profile Input------*/
	strcpy(P1.name, argv[1]);
	strcpy(P2.name, argv[2]);
	strcpy(P3.name, argv[3]);
	strcpy(P4.name, argv[4]);
	P1.score = START_SCORE;
	P2.score = START_SCORE;
	P3.score = START_SCORE;
	P4.score = START_SCORE;

/*------Finish Player Profile Input------*/

/*------Game Start------*/
	play_mode();

	final_mode();
/*------Game End------*/

	unmapper();
	close(fd);
	return 0;
}

short * mapper(off_t offset, int prot) {
	map_data[map_counter] = mmap(NULL, sizeof(short), prot, MAP_SHARED, fd, offset);
	if ( map_data[map_counter] == MAP_FAILED ) {
		fprintf(stderr, "Cannot do mmap()");
		emergency_closer();
	}
	return (short *)map_data[map_counter++];
}

void unmapper() {
	int i;
	for( i=0; i<map_counter; i++)
		munmap(map_data[i], sizeof(short));

	map_counter = 0;
}

void emergency_closer() {
	unmapper();
	close(fd);
	exit(EXIT_FAILURE);
}

/*------A collection of things to output on clcd------*/
void print_clcd(int option) {
	char clcd_str[20] = "";
	clcd_clear_display();
	clcd_set_DDRAM(0x40);

	switch (option) {
	case 0:
		strcat(clcd_str, "Wrong Input");
		clcd_write_string(clcd_str);
		break;

	case 1:
		strcat(clcd_str, "New Round");
		clcd_write_string(clcd_str);
		break;

	case 2:
		strcat(clcd_str, "Offense: ");
		strcat(clcd_str, P1.name);
		clcd_write_string(clcd_str);
		break;

	case 3:
		strcat(clcd_str, "Offense: ");
		strcat(clcd_str, P2.name);
		clcd_write_string(clcd_str);
		break;

	case 4:
		strcat(clcd_str, "Offense: ");
		strcat(clcd_str, P3.name);
		clcd_write_string(clcd_str);
		break;

	case 5:
		strcat(clcd_str, "Offense: ");
		strcat(clcd_str, P4.name);
		clcd_write_string(clcd_str);
		break;

	// Player 1: Win or Lose
	case 20:
		strcat(clcd_str, P1.name);
		strcat(clcd_str, ": Winner");
		clcd_write_string(clcd_str);
		break;

	case 21:
		strcat(clcd_str, P1.name);
		strcat(clcd_str, ": Loser");
		clcd_write_string(clcd_str);
		break;

	// Player 2: Win or Lose
	case 22:
		strcat(clcd_str, P2.name);
		strcat(clcd_str, ": Winner");
		clcd_write_string(clcd_str);
		break;

	case 23:
		strcat(clcd_str, P2.name);
		strcat(clcd_str, ": Loser");
		clcd_write_string(clcd_str);
		break;

	// Player 3: Win or Lose
	case 24:
		strcat(clcd_str, P3.name);
		strcat(clcd_str, ": Winner");
		clcd_write_string(clcd_str);
		break;

	case 25:
		strcat(clcd_str, P3.name);
		strcat(clcd_str, ": Loser");
		clcd_write_string(clcd_str);
		break;

	// Player 4: Win or Lose
	case 26:
		strcat(clcd_str, P4.name);
		strcat(clcd_str, ": Winner");
		clcd_write_string(clcd_str);
		break;

	case 27:
		strcat(clcd_str, P4.name);
		strcat(clcd_str, ": Loser");
		clcd_write_string(clcd_str);
		break;

	default:
		break;
	}
}

/*------condition for calling input mode() ------*/
truth_t logic(int p, int* wrong_key_cnt) {
	if (KEYPAD[0][p - 1] + KEYPAD[1][p - 1] + KEYPAD[2][p - 1] + KEYPAD[3][p - 1] >= 2)
		return FALSE;
	else {
		input_mode(p, &wrong_key_cnt);
		return TRUE;
	}
}

/*------Overall Process, including keypad-input------*/
void input_mode(int p, int** wrong_key_cnt) {
	int key_count, key_value;
	char clcd_str[20]="";

	key_count = keypad_read(&key_value);	// input keypad by the user

	if(key_count == 1) {

		KEYPAD[key_value / 4][p - 1] = 1;

		if (p - 1 == key_value % 4) {
			clcd_clear_display();
			strcat(clcd_str, "Exact Press!");
		}
		else {
			(**wrong_key_cnt)++;

			clcd_clear_display();
			strcat(clcd_str, "Wrong Press!");
			led_blink_all();
		}
		clcd_set_DDRAM(0x40);
		clcd_write_string(clcd_str);
	}
}

/*------show the players' score on FND------*/
void show_score() {
	int score;

	// If the score is negative, it is output on the FND in a bad way
	// So when the calculated score is less than 0, the score is changed to 0.
	if (P1.score < 0)
		P1.score = 0;
	else if (P2.score < 0)
		P2.score = 0;
	else if (P3.score < 0)
		P3.score = 0;
	else if (P4.score < 0)
		P4.score = 0;

	score = P1.score * 1000000 + P2.score * 10000 + P3.score * 100 + P4.score;

	// show output on FND
	fnd_clear();
	fnd_hexa_number(score);
}

/*------Compare keypad entered by each player and calculate scores------*/
void compare_data(int p) {
	int row, col;
	int round_score[4] = { 0 };		// preliminary deduction array for the players

	for (row = 0; row < MAX_KEY_ROW; row++) {
		for (col = 0; col < MAX_KEY_COL; col++) {
			if ((p-1 != col) && (KEYPAD[row][p - 1] + KEYPAD[row][col] == 2))
				round_score[col]++;
		}
	}

	P1.score -= round_score[0];
	P2.score -= round_score[1];
	P3.score -= round_score[2];
	P4.score -= round_score[3];
}

/*------Initialize the KEYPAD Array with 0------*/
void clear_KEYPAD_Arr() {
	int row, col;

	for (row = 0; row < MAX_KEY_ROW; row++) {
		for (col = 0; col < MAX_KEY_COL; col++) {
			KEYPAD[row][col] = 0;
		}
	}
}

/*------Enter keypad input status for each player------*/
int PRESS(int p) {
	int wrong_key_cnt = 0;

	switch (p) {
	case 1:		// Player 1
		dot_clear();		// dot matrix initialization
		dot_write(0);		// show 'A' on dot matrix
		break;

	case 2:		// Player 2
		dot_clear();		// dot matrix initialization
		dot_write(1);		// show 'B' on dot matrix
		break;

	case 3:		// Player 3
		dot_clear();		// dot matrix initialization
		dot_write(2);		// show 'C' on dot matrix
		break;

	case 4:		// Player 4
		dot_clear();		// dot matrix initialization
		dot_write(3);		// show 'D' on dot matrix
		break;

	default:
		break;
	}

	// iteration to call input_mode(enable keypad input)
	while(logic(p, &wrong_key_cnt) == TRUE) {}

	// Return a deduction if the player used both oppportunities
	// Return 0 deduction point unless the player are eligible for deduction
	if (KEYPAD[0][p - 1] + KEYPAD[1][p - 1] + KEYPAD[2][p - 1] + KEYPAD[3][p - 1] == 2)
		return wrong_key_cnt;
}

/*------Distinguish the winner and the loser and they are shown on the CLCD------*/
void final_mode() {
	char clcd_str_A[20] = "";
	char clcd_str_B[20] = "";
	char clcd_str_C[20] = "";
	char clcd_str_D[20] = "";

	/*------Player A result------*/
	if (P1.score != 0)		// winner: 1, loser: 0
		print_clcd(20);
	else
		print_clcd(21);
	usleep(1500000);

	/*------Player B result------*/
	if (P2.score != 0)
		print_clcd(22);
	else
		print_clcd(23);
	usleep(1500000);

	/*------Player C result------*/
	if (P3.score != 0)
		print_clcd(24);
	else
		print_clcd(25);
	usleep(1500000);

	/*------Player D result------*/
	if (P4.score != 0)
		print_clcd(26);
	else
		print_clcd(27);
	usleep(1500000);
}

/*------Game Play Mode------*/
int play_mode() {
	int n_round = 0;
	int turn;

	show_score();

	while (1) {
		clear_KEYPAD_Arr();

		turn = n_round % 4;
		print_clcd(1);				// new round
		usleep(3000000);

		switch (turn) {
		case 0:	// Player 1 is an offense.
			print_clcd(2);
			P1.score = P1.score - PRESS(1);		// If Player 1 presses another player's keypad, his/her score will be deducted.
			P2.score = P2.score - PRESS(2);		// If Player 2 presses another player's keypad, his/her score will be deducted.
			P3.score = P3.score - PRESS(3);		// If Player 3 presses another player's keypad, his/her score will be deducted.
			P4.score = P4.score - PRESS(4);		// If Player 4 presses another player's keypad, his/her score will be deducted.
			compare_data(1);
			break;

		case 1: // Player 2 is an offense.
			print_clcd(3);
			P2.score = P2.score - PRESS(2);		// If Player 2 presses another player's keypad, his/her score will be deducted.
			P3.score = P3.score - PRESS(3);		// If Player 3 presses another player's keypad, his/her score will be deducted.
			P4.score = P4.score - PRESS(4);		// If Player 4 presses another player's keypad, his/her score will be deducted.
			P1.score = P1.score - PRESS(1);		// If Player 1 presses another player's keypad, his/her score will be deducted.
			compare_data(2);
			break;

		case 2:	// Player 3 is an offense.
			print_clcd(4);
			P3.score = P3.score - PRESS(3);		// If Player 3 presses another player's keypad, his/her score will be deducted.
			P4.score = P4.score - PRESS(4);		// If Player 4 presses another player's keypad, his/her score will be deducted.
			P1.score = P1.score - PRESS(1);		// If Player 1 presses another player's keypad, his/her score will be deducted.
			P2.score = P2.score - PRESS(2);		// If Player 2 presses another player's keypad, his/her score will be deducted.
			compare_data(3);
			break;

		case 3:	// Player 4 is an offense.
			print_clcd(5);
			P4.score = P4.score - PRESS(4);		// If Player 4 presses another player's keypad, his/her score will be deducted.
			P1.score = P1.score - PRESS(1);		// If Player 1 presses another player's keypad, his/her score will be deducted.
			P2.score = P2.score - PRESS(2);		// If Player 2 presses another player's keypad, his/her score will be deducted.
			P3.score = P3.score - PRESS(3);		// If Player 3 presses another player's keypad, his/her score will be deducted.
			compare_data(4);
			break;

		default:
			break;
		}

		// show the N-Round result on FND
		show_score();

		// If even one of them scores zero, the game is over.
		if (P1.score == 0 || P2.score == 0 || P3.score == 0 || P4.score == 0)
			break;
		// Otherwise, next round
		else
			n_round++;
	}
}
