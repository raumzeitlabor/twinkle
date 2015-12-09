/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                             Tobias Rehbein
 * ----------------------------------------------------------------------------
 */

#define F_CPU	8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define PWM_PORT	(PORTB)
#define PWM_DDR		(DDRB)
#define PWM_STEPS	(70)
#define PWM_COUNTER	pwmcnt
#define PWM_TMPPORT	pwmtmpport
#include "softpwm.h"

#define TIMER_NOPRESCALER	(1<<CS00)

#define LED1	(1<<PB0)
#define LED2	(1<<PB1)
#define LED3	(1<<PB2)
#define LED4	(1<<PB3)
#define LED5	(1<<PB4)
#define LED6	(1<<PB5)
#define LED7	(1<<PB6)

// jot 70 0 255 | xargs -I% python -c 'print int(round(255.0 * pow(%/255.0, 2.2)))'
static uint8_t PWM_LEN[] = {
	  0,    0,    0,    0,    1,    1,    1,    2,    2,    3,
	  4,    5,    5,    6,    8,    9,   10,   12,   13,   15,
	 17,   19,   20,   23,   25,   27,   30,   33,   35,   38,
	 41,   44,   47,   50,   54,   57,   61,   65,   68,   73,
	 77,   82,   85,   90,   95,   99,  105,  110,  114,  120,
	126,  130,  137,  143,  149,  154,  161,  168,  173,  181,
	188,  194,  201,  209,  217,  223,  231,  240,  246,  255,
};

PWM_CREATE(0, LED1, 1,  3);
PWM_CREATE(1, LED2, 1, 15);
PWM_CREATE(2, LED3, 1, 38);
PWM_CREATE(3, LED4, 1, 73);
PWM_CREATE(4, LED5, 1, 120);
PWM_CREATE(5, LED6, 1, 181);
PWM_CREATE(6, LED7, 1, 255);

ISR(TIMER0_COMPA_vect)
{
	static uint8_t pwmcnt = 0;

	PWM_TICK_BEGIN();

	PWM_TICK(0);
	PWM_TICK(1);
	PWM_TICK(2);
	PWM_TICK(3);
	PWM_TICK(4);
	PWM_TICK(5);
	PWM_TICK(6);

	PWM_TICK_END();

	if (++pwmcnt == PWM_STEPS)
	{
		pwmcnt = 0;
		TCNT0 = 0;
		OCR0A = 0;
	}

	OCR0A += PWM_LEN[pwmcnt];
}

int
main(void)
{
	PWM_INIT(0);
	PWM_INIT(1);
	PWM_INIT(2);
	PWM_INIT(3);
	PWM_INIT(4);
	PWM_INIT(5);
	PWM_INIT(6);

	TCCR0B |= TIMER_NOPRESCALER;
	TIMSK |= (1<<OCIE0A);

	sei();

	for (;;)
	{
		PWM_UPDATE(0);
		PWM_UPDATE(1);
		PWM_UPDATE(2);
		PWM_UPDATE(3);
		PWM_UPDATE(4);
		PWM_UPDATE(5);
		PWM_UPDATE(6);

		_delay_ms(50);
	}

	/* NOTREACHED */
	return (0);
}
