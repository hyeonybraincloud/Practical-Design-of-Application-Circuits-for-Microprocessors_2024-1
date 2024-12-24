//-------| src/dot.c |-------//
#include "dot.h"


static unsigned short dot_decimal[10][MAX_DOT] = {
	{0x3f, 0x48, 0x48, 0x48, 0x3F}, // 0 A
	{0x7f, 0x49, 0x49, 0x49, 0x36}, // 1 B
	{0x3e, 0x41, 0x41, 0x41, 0x41}, // 2 C
	{0x7f, 0x41, 0x41, 0x41, 0x3e}, // 3 D
	{0x7f, 0x7f, 0x7f, 0x7f, 0x7f}, // 4 Full lighting
	{0x79, 0x49, 0x49, 0x49, 0x4f}, // 5
	{0x7f, 0x49, 0x49, 0x49, 0x4f}, // 6
	{0x40, 0x40, 0x40, 0x40, 0x7f}, // 7
	{0x7f, 0x49, 0x49, 0x49, 0x7f}, // 8
	{0x78, 0x48, 0x48, 0x48, 0x7f}  // 9
};

static short * dot[MAX_DOT];

/*------Bring the address mapped------*/
void init_dot(short * address[]) {
	int i;
	for( i=0; i<MAX_DOT; i++ ) {
		dot[i] = address[i];
	}
}

/*------Erase the dot matrix------*/
void dot_clear() {
	int i;
	for(i=0; i<MAX_DOT; i++){
		*dot[i] = 0;
	}
	usleep(0); // for Ximulator
}

/*------Write something on the dot matrix------*/
void dot_write(int number) {
	int i;
	for(i=0; i<MAX_DOT; i++){
		*dot[i] = dot_decimal[number][i];
	}
	usleep(0); // for Ximulator
}