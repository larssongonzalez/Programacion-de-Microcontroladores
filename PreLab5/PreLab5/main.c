/*************************************************************
 * Universidad del Valle de Guatemala
 * IE2023 Programacion de Microcontroladores
 * PreLab5.c
 * Autor: Larsson Gonzalez
 * Proyecto:
 * Hardware: ATMega328p
 * Creado: 12/4/2024 08:17:01
 *************************************************************/ 
#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PWMT1/PWMT1.h"

//************************************************************
// DECLARACION DE FUNCIONES
//************************************************************
void ADC_init(void);

//************************************************************
// DECLARACION DE VARIABLES
//************************************************************
uint8_t adcread;
uint8_t servo1;
//************************************************************
// SETUP
//************************************************************
int main(void){
	
	cli();
	PWMT1FastInitA(invertido, 40);
	PWMT1FastInitB(noinvertido, 40);
	ADC_init();
	
	updateDutyCycleA(50);
	updateDutyCycleB(50);
	
	sei();
	
    //********************************************************
	// LOOP
	//********************************************************
    while (1) {
		/*servo1 = (20*0xFF)/adcread;
		
		updateDutyCycleA(servo1);*/
		
		updateDutyCycleA(50);
		updateDutyCycleB(50);
    }
}

//************************************************************
// ISR
//************************************************************
ISR(ADC_vect){
	adcread = ADCH;
	ADCSRA |= (1<<ADIF);
}

//************************************************************
// FUNCIONES
//************************************************************

void ADC_init(){
	// Seleccion de Canal ADC (A)
	ADMUX = 0x07;
	
	// Utilizando AVCC = 5V internos
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	// Justificacion a la Izquierda
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	
	// Habilitando el ADC
	ADCSRA |= (1<<ADEN);
	
	//Habilitamos las interrupciones
	ADCSRA |= (1<<ADIE);
	
	// Habilitamos el Prescaler de 128
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	DIDR0 |= (1<<ADC0D);
}