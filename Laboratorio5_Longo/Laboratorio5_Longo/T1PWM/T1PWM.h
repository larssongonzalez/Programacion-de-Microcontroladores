/*
 * T1PWM.h
 *
 * Created: 12/4/2024 11:56:51
 *  Author: Longo
 */ 


#ifndef T1PWM_H_
#define T1PWM_H_
#include <stdint.h>
#include <avr/io.h>
#define invertido 1
#define no_invertido 0
/*Funcion de Configuracion  Canal A*/
void T1PWM_FastInitA(uint8_t inverted, uint16_t prescaler);

/*Funcion de Configuracion  Canal B*/
void T1PWM_FastInitB(uint8_t inverted, uint16_t prescaler);

/*Frecuencia y DutyCycle*/
void updateDutyCycleA(int frecuencia, int duty);

/*Frecuencia y DutyCycle*/
void updateDutyCycleB(int frecuencia, int duty);




#endif /* T1PWM_H_ */