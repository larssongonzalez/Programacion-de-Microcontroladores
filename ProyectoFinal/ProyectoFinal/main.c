/*
 * ProyectoFinal.c
 *
 * Created: 17/5/2024 17:08:36
 * Author : polst
 */ 
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWMS/PWM0.h"
#include "PWMS/PWM1.h"
#include "PWMS/PWM2.h"

/************************************************************************************
*	DECLARACION DE VARIABLES
*************************************************************************************/
float adcvalue1 = 0;
float adcvalue2 = 0;
float adcvalue3 = 0;
float adcvalue4 = 0;

// VARIABLES PARA GUARDAR LOS ESTADOS DE LOS BOTONES
volatile int state = 0;
volatile int buttonStateD3 = 0;
volatile int buttonStateD2 = 0;
volatile int buttonStateA4 = 0;
volatile int buttonStateA5 = 0;

/************************************************************************************
*	DECLARACION DE FUNCIONES
*************************************************************************************/
void initADC(void);	// CONFIGURACION DEL ADC
uint16_t adcread(uint8_t canal);	// SELECCION DE CANAL DE ADC
void initUART(void);	//CONFIGURACION DE UART
void EEPROM_write(uint16_t uiAddress, uint16_t ucData);		// FUNCION PARA ESCRIBIR EN EEPROM
uint16_t EEPROM_read(uint16_t uiAddress);		// FUNCION PARA LEER DE EEPROM
void writeTextUART(char * Texto);		// FUNCION PARA ESCRIBIR UNA CADENA DE CARACTERES
void writeUART(char Caracter);		// FUNCION PARA ESCRIBIR UN CARACTER




int main(void)
{
	cli();
	
	// Configura los botones como entrada
	DDRD &= ~(1<<DDD4) & ~(1<<DDD3) & ~(1<<DDD2); // PIND PD4, PD3, PD2 como entrada
	PORTD |= (1<<PORTD4) | (1<<PORTD3) | (1<<PORTD2); // Habilita Pull-Up
	
	DDRC &= ~(1<<DDC4) & ~(1<<DDC5); // PINC PC4, PC5 (A4, A5) como entrada
	PORTC |= (1<<PORTC4) | (1<<PORTC5); // Habilita Pull-Up
	
	//Configura los leds como salida
	DDRB |= (1 << DDB4); // Pin PB4 como salida
	DDRB |= (1<<DDB0); // Pin PB0 como salida
	DDRD |= (1<<DDD7); // Pin PD7 como salida
	
	// Configura la interrupcion PCINT2
	PCICR |= (1 << PCIE2);			    // Habilita la interrupcion PCINT2
	PCMSK2 |= (1 << PCINT20) | (1 << PCINT19) | (1 << PCINT18); // Habilita PCINT20 (PD4), PCINT19 (PD3), PCINT18 (PD2)
	
	// Configura la interrupcion PCINT1
	PCICR |= (1 << PCIE1); // Habilita la interrupcion PCINT1
	PCMSK1 |= (1 << PCINT12) | (1 << PCINT13); // Habilita PCINT12 (PC4), PCINT13 (PC5)
	
    initADC();
	initUART();
	initPWM1();
	initPWM0A(no_invertido, 1024);
	initPWM2A(no_invertido, 1024);
	
	
	sei();
	
    while (1) 
    {
		/**************MODO MANUAL***************/	
		if (state == 0)
		{
			// Led indicador de estado
			PORTB |= (1<<PORTB4);
			PORTB &= ~(1<<PORTB0);
			PORTD &= ~(1<<PORTD7);
			
			// Parte manual de ADC y PWM
			// ADC0(A0) - OC1A(D9 - PB1) - TIMER 1
			adcvalue1 = adcread(0);
			writecanalA(adcvalue1); // CANAL A
			_delay_ms(10);
			
			// ADC1(A1) - OC1B(D10 - PB2) - TIMER 1
			adcvalue2 = adcread(1);
			writecanalB(adcvalue2); // CANAL B
			_delay_ms(10);
			
			// ADC2(A2) - OC2A(D11 - PB3) - TIMER 2
			adcvalue3 = adcread(2);
			updateDutyCA2(adcvalue3);	// CANAL A
			_delay_ms(10);
			
			// ADC3(A3) - OC0A(D6 - PD6) - TIMER 0
			adcvalue4 = adcread(3);
			updateDutyCA(adcvalue4);	// CANAL 4
			_delay_ms(10);
			
		/**************MODO EEPROM***************/	
		} else if (state == 1)
		{
			// Led indicador de estado
			PORTD &= ~(1<<PORTD7);
			PORTB &= ~(1<<PORTB4);
			PORTB |= (1<<PORTB0);
		
		/**************MODO ADAFRUIT IO***************/		
		} else if (state == 2)
		{
			// Led indicator de estado
			PORTB &= ~(1<<PORTB4);
			PORTB &= ~(1<<PORTB0);
			PORTD |= (1<<PORTD7);
			
		} else if (state == 3)
		{
			// Led indicator de estado
			PORTB &= ~(1<<PORTB4);
			PORTB &= ~(1<<PORTB0);
			PORTD &= ~(1<<PORTD7);
		}
		
			
		
		/*adcvalue1 = adcread(0);
		writecanalA(adcvalue1);
		_delay_ms(10);
		
		adcvalue2 = adcread(1);
		writecanalB(adcvalue2);
		_delay_ms(10);
		
		adcvalue3 = adcread(2);
		updateDutyCA2(adcvalue3);
		_delay_ms(10);
		
		adcvalue4 = adcread(3);
		updateDutyCA(adcvalue4);
		_delay_ms(10);*/
		
    }
}

/************************************************************************************
*	FUNCIONES
*************************************************************************************/
void initADC(void){
	ADMUX |= (1<<REFS0);	// VREF = AVCC
	ADMUX &= ~(1<<REFS1);	// VREF = AVCC
	ADMUX &= ~(1<<ADLAR);	// 10bits
	
	// Prescaler 128 - 16M/128 = 125kHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	ADCSRA |= (1<<ADEN);	// ON ADC
	
	// DESABILITAR ENTRADAS DIGITALES DE PC0, PC1 Y PC2.
	DIDR0 |= (1<<ADC0D)|(1<<ADC1D)|(1<<ADC2D);
}

uint16_t adcread(uint8_t canal){
	ADMUX = (ADMUX & 0xF0)|canal;	// SELECCIONA EL CANAL A USAR
	ADCSRA |= (1<<ADSC);	// INICIA LA CONVERSION DEL ADC
	while ((ADCSRA)&(1<<ADSC));	// BUCLE DE QUE HASTA QUE FINALICE LA CONVERSION
	return(ADC);
}

void EEPROM_write(uint16_t uiAddress, uint16_t ucData){
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

uint16_t EEPROM_read(uint16_t uiAddress){
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

void initUART(void){
	//Paso 1: RX y TX como salida
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	//Paso 2:Configurar UCSR0A
	UCSR0A = 0;
	
	//Paso 3: configurar UCSR0B donde se habilita ISR de recepcion y se habilita RX y TX
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	//Paso 4: Configurar UCSR0C, asincrono, paridad: none, 1 bit stop, data bits de 8 bits
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	
	//Paso 5: Configurar velocidad de baudrate a 9600
	UBRR0 = 103;
}

//Función de escritura de una cadena de caracteres
void writeTextUART(char * Texto){
	uint8_t o;
	for(o = 0; Texto[o]!= '\0'; o++){
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = Texto[o];
	}
}


//Funcion de escritura de un caracter
void writeUART(char data){
	while(!(UCSR0A & (1<<UDRE0)));  //hasta que la bandera este en 1
	UDR0 = data;
	
}
/************************************************************************************
*	INTERRUPCIONES
*************************************************************************************/
ISR(PCINT2_vect){
	if (!(PIND & (1<<PIND4)))
	{
		if (state < 2)
		{
			state++;
			} else {
			state = 0;
		}
	}
	
	// Interrupciones para los botones PD3, PD2 - Botones para guardar posiciones
	if (!(PIND & (1<<PIND3)))
	{
		if (buttonStateD3 < 3)
		{
			buttonStateD3++;
		}
		else
		{
			buttonStateD3 = 0;
		}
	}
	if (!(PIND & (1<<PIND2)))
	{
		if (buttonStateD2 < 3)
		{
			buttonStateD2++;
		}
		else
		{
			buttonStateD2 = 0;
		}
	}
	
	if (buttonStateD3 == 1){		/*Guarda la primera posicion*/
		EEPROM_write(0, adcvalue1);
		EEPROM_write(1, adcvalue2);
		EEPROM_write(2, adcvalue3);
		} else if (buttonStateD2 == 1){	/*Guarda la segunda posicion*/
		EEPROM_write(3, adcvalue2);
		EEPROM_write(4, adcvalue3);
		EEPROM_write(5, adcvalue4);
		} else if (buttonStateD3 == 2){
		adcvalue1 = EEPROM_read(0);
		writecanalA(adcvalue1);
		adcvalue2 = EEPROM_read(1);
		writecanalB(adcvalue2);
		adcvalue3 = EEPROM_read(2);
		updateDutyCA2(adcvalue3);
		} else if (buttonStateD2 == 2){
		adcvalue2 = EEPROM_read(3);
		writecanalB(adcvalue2);
		adcvalue3 =  EEPROM_read(4);
		updateDutyCA2(adcvalue3);
		adcvalue4 = EEPROM_read(5);
		updateDutyCA(adcvalue4);
	}
}

ISR(PCINT1_vect){
	// Interrupciones para los botones A4(PC4) , A5(PC5)
	if (!(PINC & (1<<PINC4)))
	{
		if (buttonStateA4 < 3)
		{
			buttonStateA4++;
		}
		else
		{
			buttonStateA4 = 0;
		}
	}
	if (!(PINC & (1<<PINC5)))
	{
		if (buttonStateA5 < 3)
		{
			buttonStateA5++;
		}
		else
		{
			buttonStateA5 = 0;
		}
	}
	
	if (buttonStateA4 == 1){	//Guarda la tercera posicion
		EEPROM_write(6, adcvalue3);
		EEPROM_write(7, adcvalue4);
		EEPROM_write(8, adcvalue1);
		} else if (buttonStateA5 == 1){	//Guarda la cuarta posicion
		EEPROM_write(9, adcvalue1);
		EEPROM_write(10, adcvalue2);
		EEPROM_write(11, adcvalue3);
		EEPROM_write(12, adcvalue4);
		} else if (buttonStateA4 == 2){
		adcvalue3 = EEPROM_read(6);
		updateDutyCA2(adcvalue3);
		adcvalue4 = EEPROM_read(7);
		updateDutyCA(adcvalue4);
		adcvalue1 = EEPROM_read(8);
		} else if (buttonStateA5 == 2){
		adcvalue1 = EEPROM_read(9);
		writecanalA(adcvalue1);
		adcvalue2 = EEPROM_read(10);
		writecanalB(adcvalue2);
		adcvalue3 = EEPROM_read(11);
		updateDutyCA2(adcvalue3);
		adcvalue4 = EEPROM_read(12);
		updateDutyCA(adcvalue4);
	}
}