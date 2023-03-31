/* 
 * Archivo: Lab06.c
 * Dispositivo: PIC16F887
 * Autor: Carlos Julio Valdés Oajaca
 * Compilador: XC8, MPLABX v6.05
 
 * Programa: leyendo 2 POTS en portE y desplegando en portA y portB
 * Hardware: 

 * Creado: 26 de marzo, 2023
 * Última modificación: 26 de marzo, 2023
 */

// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <pic16f887.h>
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000 //4MHz
///////////////////////////////////DEFINIR CONSTANTES///////////////////////////////////////////////////////////////////////////////////
//#define _tmr0_value 61 //50ms
////////////////////////////////////VARIABLES GLOBALES/////////////////////////////////////////////////////////////////////////////////
//uint8_t var;
/////////////////////////////////////PROTOTIPOS DE FUNCIONES///////////////////////////////////////////////////////////////////////////
void setup(void);
/////////////////////////////////////INTERRUPCIONES//////////////////////////////////////////////////////////////////////////////////
void __interrupt() isr(void)
{
  if (PIR1bits.ADIF) //verifica si hay rebalse del tmr0
  {
    if (ADCON0bits.CHS == 6)
        PORTB = ADRESH;
    else
        PORTA = ADRESH;
    PIR1bits.ADIF = 0;
  }      
}
///////////////////////////////////////CODIGO PRINCIPAL/////////////////////////////////////////////////////////////////////
/*void delay_ms(unsigned int ms)
{
    unsigned int i;
    
    for (i = 0; i < ms; i++)
    {
        __delay_ms(1); // llama a la funcion de delay del compilador
    }

}
*/
void main(void) {
    setup(); //llamar a la configuracion
    ADCON0bits.GO = 1;
    
 ////LOOP PRINCIPAL/////   
    while(1) //Loop infinito
    {
        if (ADCON0bits.GO == 0)
        {
            if (ADCON0bits.CHS == 6)
            
                ADCON0bits.CHS = 5;
            else 
                 ADCON0bits.CHS = 6;
            __delay_us(50);    
            ADCON0bits.GO = 1;    
        }
    }
    
}

//////////////////////////////////////FUNCIONES (SETUP)/////////////////////////////////////////////////////////////////////
void setup(void)
{
    //configuracion de pines y puertos    
    ANSEL= 0b01100000;  //configura los pines de los POT como analogos
    ANSELH= 0; //configura los pines de portB como digital
    
    TRISA = 0; // Configura el portA como salida
    TRISB = 0; //Configura portB como salida
    TRISE = 0b011; //Configurar entradas de los POT
    
    PORTA = 0;
    PORTB = 0;
    PORTE = 0;
    
    //configuracion oscilador
    OSCCONbits.IRCF = 0b0110; //4MHz
    OSCCONbits.SCS = 1;       //Utilizar el reloj interno
    
    //configuracion del ADC
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS = 0b01;
    ADCON0bits.CHS = 5;
    ADCON0bits.ADON = 1;
    __delay_us(50);
    
    //configuracion de interrupciones
    PIR1bits.ADIF = 0;   //Inicializar bandera en 0
    PIE1bits.ADIE = 1;   //Habilitar interrupciones analogas
    INTCONbits.PEIE = 1; //Habilitar interrupciones perifericas
    INTCONbits.GIE = 1; // Habilita interrupciones generales

    return;   

}