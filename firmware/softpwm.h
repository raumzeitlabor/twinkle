/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                             Tobias Rehbein
 * ----------------------------------------------------------------------------
 */

#ifndef PWM_PORT
#error "PWM_PORT not defined."
# endif

#ifndef PWM_DDR
#error "PWM_DDR not defined."
#endif

#ifndef PWM_STEPS
#error "PWM_STEPS not defined."
#endif

#ifndef PWM_COUNTER
#error "PWM_COUNTER not defined."
#endif

#ifndef PWM_TMPPORT
#error "PWM_TMPPORT not defined."
#endif

#define PWM_INIT(c)	PWM_DDR |= pwm##c.bit

#define PWM_CREATE(n, b, d, dc)						\
	struct pwm pwm##n = {						\
		.direction = d,						\
		.dutycycle = dc,					\
		.bit = b						\
	}

#define PWM_TICK_BEGIN()	uint8_t PWM_TMPPORT = 0x00

#define PWM_TICK(n)	{						\
		if (pwm##n.dutycycle > PWM_COUNTER)			\
			PWM_TMPPORT |= pwm##n.bit;			\
	} while (0)

#define PWM_TICK_END()	PWM_PORT =  PWM_TMPPORT

#define PWM_UPDATE(n)	{						\
		if (pwm##n.dutycycle >= PWM_STEPS - 1)			\
			pwm##n.direction = -1;				\
		else if (pwm##n.dutycycle <= 0)				\
			pwm##n.direction = 1;				\
		pwm##n.dutycycle += pwm##n.direction;			\
	} while (0)

struct pwm {
	int8_t direction;
	volatile uint8_t dutycycle;
	uint8_t bit;
};
