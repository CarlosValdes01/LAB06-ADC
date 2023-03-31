// Autor: Johann Haeussler 21223
// Fecha: 26.03.23
// Microcontrolador: PIC16F887

// CONFIG1
#pragma config FOSC     =   INTRC_NOCLKOUT  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE     =   OFF             // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE    =   OFF             // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE    =   OFF             // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP       =   OFF             // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD      =   OFF             // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN    =   OFF             // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO     =   OFF             // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN    =   OFF             // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP      =   OFF             // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V    =   BOR40V          // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT      =   OFF             // Flash Program Memory Self Write Enable bits (Write protection off)

#include <pic16f887.h>
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL

void __interrupt() isr(void) // Declaraci�n de la funci�n de interrupci�n
{ 
    if (T0IF) // Verifica si la bandera de interrupci�n del Timer0 est� activa
    {
        PORTAbits.RA0 = ~PORTAbits.RA0; // Invierte el estado del pin RA0
        TMR0 = 56; // Recarga el valor del Timer0 para generar una interrupci�n en 200 us
        T0IF = 0; // Limpia la bandera de interrupci�n del Timer0
    }
}

void delay_ms(uint16_t ms) // Funci�n para generar retardos en milisegundos
{
    for (uint16_t i = 0; i < ms; i++)
    {
        __delay_ms(1); // Llamada a la funci�n de retardos del compilador
    }
}

void setup(void) // Funci�n para configurar los registros del microcontrolador
{
    ANSEL  = 0; // Configura los pines como entradas/salidas digitales
    ANSELH = 0;
    TRISA0 = 0; // Configura el pin RA0 como salida digital
    
    // Configurando el oscilador 
    OSCCONbits.IRCF = 0b111; // Configura el oscilador interno a 8MHz
    OSCCONbits.SCS = 1; // Utiliza el oscilador interno para el reloj del sistema
    
    // Configurando el timer0
    OPTION_REGbits.T0CS = 0; // Selecciona el oscilador interno para el Timer0
    OPTION_REGbits.PSA = 0; // Habilita el preescalador del Timer0
    OPTION_REGbits.PS = 0b111; // Configura el preescalador del Timer0 a 1:256
    TMR0 = 56; // Carga el valor inicial del Timer0 para generar una interrupci�n en 200 us
    
    // Configurando la interrupci�n del TMR0
    INTCONbits.T0IF = 0; // Limpia la bandera de interrupci�n del Timer0
    INTCONbits.T0IE = 1; // Habilita la interrupci�n del Timer0
    INTCONbits.GIE = 1; // Habilita las interrupciones generales
}

void main(void) 
{ 
    setup(); // Llamada a la funci�n de configuraci�n
    
    while(1) // Ciclo infinito
    {
        PORTA++; // Incrementa el valor del puerto A en 1
        delay_ms(165); // Genera un retardo de 165 ms
    }
  ��
����return;
}