/*
 * PWM0.h
 *
 * Created: 11/5/2024 11:17:47
 *  Author: polst
 */ 


#ifndef PWM0_H_
#define PWM0_H_
#include <avr/io.h>
#include <stdint.h>

#define invertido 0
#define no_invertido 1

void PWM0_init(void);
void PWM0_dca(uint8_t dc, uint8_t modo);
void PWM0_dcb(uint8_t dc, uint8_t modo);
float map0(float x, float in_min, float in_max, float out_min, float out_max);





#endif /* PWM0_H_ */