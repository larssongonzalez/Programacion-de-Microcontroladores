/*
 * Laboratorio5_Longo.c
 *
 * Created: 12/4/2024 11:55:28
 * Author : Longo
 */ 

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "T1PWM/T1PWM.h"

//************************************************************
// SETUP
//************************************************************
int main(void)
{
	
	
	T1PWM_FastInitA(invertido, 1024);
	T1PWM_FastInitB(no_invertido, 1024);
	
	int frecuency = 120;	
	int dutyCycleA = 75;	// Duty Cycle Channel A (50%)
	int dutyCycleB = 25;	
	
    
    while (1) {
		
		
		updateDutyCycleA(frecuency, dutyCycleA);
		updateDutyCycleB(frecuency, dutyCycleB);
		
		_delay_ms(50);
		
		
		dutyCycleA += 10;
		if (dutyCycleA > 100){
			dutyCycleA = 0;	// Reiniciar ciclo de trabajo
		}
		
		dutyCycleB += 5;
		if (dutyCycleB > 100){
			dutyCycleB = 0;	// Reiniciar ciclo de trabajo
		}
		
    }
}
