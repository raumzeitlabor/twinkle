Firmware for Project Twinkle
============================

This firmware was written for an ATtiny2313. It does nothing but
generate pulsating linear PWM signals on `PB0` to `PB6`.

* `main.c` - the main part of Project Twinkles firmware.
* `softpwm.h` - macros and structs to support the PWM generation in software
