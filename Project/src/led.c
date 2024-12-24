//-------| src/led.c |-------//
#include "led.h"

static short * led;

void init_led(short * address) {
	led = address;
}

void led_blink_all() {
	int i;
	for( i=1; i<=16 ; i++ ) {
		if ( i%2 == 1 ) {
			*led = (short)~0xFF;
		}
		else {
			*led = (short)~0x00;
		}
		usleep(100000);
	}
}

void led_clear() {
	*led = (short)~0;
}

void led_all() {
	*led = (short)~0xFF;
}

void led_bit(int number) {
	*led = (short)~(number << 4);
}