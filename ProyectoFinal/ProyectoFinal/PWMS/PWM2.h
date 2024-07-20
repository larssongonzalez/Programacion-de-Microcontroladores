/*
 * PWM2.h
 *
 * Created: 17/5/2024 17:13:31
 *  Author: polst
 */ 


#ifndef PWM2_H_
#define PWM2_H_
#include <avr/io.h>
#include <stdint.h>

#define invertido 1
#define no_invertido 0

void initPWM2A(uint8_t inverted, uint16_t prescaler);
void updateDutyCA2(uint8_t duty);
float map2(float x, float in_min, float in_max, float out_min, float out_max);




#endif /* PWM2_H_ */