/*
 * PWMT1.c
 *
 * Created: 12/4/2024 09:04:57
 *  Author: polst
 */ 

#define F_CPU 16000000UL
#include "PWMT1.h"
#include <util/delay.h>
int _freq = 0;



void PWMT1FastInitA(uint8_t inverted, uint8_t freq){
	//Pin Configuration OC1A
	DDRB |= (1<<DDB1);
	
	// Configurar el modo de salida PWM
	if (inverted){
		//Configuracion de modo Invertido
		TCCR1A |= (1<<COM1A1)|(1<<COM1A0); // PWM invertido
	} 
	else{
		//Configuracion de modo No Invertido
		TCCR1A |=  (1<<COM1A1);		// PWM no invertido
		TCCR1A &= ~(1<<COM1A0);
	}
	
	
	//Configuracion de Modo Fast PWM, TOP =	ICR1
	TCCR1A |=  (1<<WGM11);
	TCCR1B |=  (1<<WGM13)|(1<<WGM12); 
	
	
	//Configuracion de Prescaler 1024
	TCCR1B &= ~(1<<CS11)|(1<<CS10);
	TCCR1B |=  (1<<CS12);
	
	
	_freq = freq;
	ICR1 = (F_CPU/1024/_freq);
	
}

void PWMT1FastInitB(uint8_t inverted, uint8_t freq){
	//Pin Configuration OC1B
	DDRB |= (1<<DDB2);
	
	if (inverted){
		//Configuracion de modo Invertido
		TCCR1A |= (1<<COM1B1)|(1<<COM1B0);	// PWM invertido
		
	} 
	else{
		//Configuracion de modo No Invertido
		TCCR1A |=  (1<<COM1B1); // PWM no invertido
		TCCR1A &= ~(1<<COM1B0);
	}
	
	
	//Configuracion de Modo Fast PWM, TOP = ICR1
	TCCR1A |=  (1<<WGM13);
	TCCR1A |=  (1<<WGM12);
	TCCR1A |=  (1<<WGM11);
	
	//Configuracion de Prescaler 1024
	TCCR1B &= ~((1<<CS11)|(1<<CS10));
	TCCR1B |=  (1<<CS12);
	
	
	_freq = freq;
	ICR1 = (F_CPU/1024/_freq)-1;
}

void updateDutyCycleA(int duty){
	OCR1A = (((F_CPU/1024/ _freq) - 1)* duty)/100;
}

void updateDutyCycleB(int duty){
	OCR1B = (((F_CPU/1024/ _freq) - 1)* duty)/100;
	
}