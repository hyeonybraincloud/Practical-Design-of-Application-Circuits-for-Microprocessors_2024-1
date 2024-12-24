//-------| lib/led.h |-------//
#ifndef	LED_H
#define	LED_H 


#include <unistd.h> // usleep()


void init_led(short * address);
void led_blink_all(void);
void led_clear(void);
void led_all(void);
void led_bit(int number);

#endif