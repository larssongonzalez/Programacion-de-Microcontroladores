/*********************************************************
 * Universidad del Valle de Guatemala
 * IE2023 Programacion de Microcontroladores
 * Lab5.c
 * Autor: Larsson Gonzalez
 * Proyecto:
 * Hardware:
 * Created: 12/4/2024 16:37:02
 *********************************************************/ 
#define F_CPU 16000000UL
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PWMT2/PWMT2.h"


int main(void)
{
	cli();
    pwm_init();
	servo_set(0,180);
	sei();
	
	uint16_t i = 0;

    while (1){
		for(i = 0; i <= 180; i++){
			servo_set(i,180);
			_delay_ms(40);
		}
		for(i = 180; i >= 0; i--){
			servo_set(i,180);
			_delay_ms(40);
		}
		
    }
}

