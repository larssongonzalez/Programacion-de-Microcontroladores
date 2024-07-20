/*
 * PWMT2.h
 *
 * Created: 12/4/2024 16:50:59
 *  Author: polst
 */ 


#ifndef PWMT2_H_
#define PWMT2_H_

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PWM_TOP (39999u)
#define SERVO_MIN (1999u)
#define SERVO_MAX (4999u)

void pwm_init(void);
void pwm_sweep(void);
void servo_set(uint16_t deg, uint16_t max_deg);

#endif /* PWMT2_H_ */