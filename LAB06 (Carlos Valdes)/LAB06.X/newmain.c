/* 
 * File:   newmain.c
 * Author: carlo
 *
 * Created on March 23, 2023, 5:59 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
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

///////////////////////////////////DEFINIR CONSTANTES///////////////////////////////////////////////////////////////////////////////////
#define _tmr0_value 61
////////////////////////////////////VARIABLES GLOBALES/////////////////////////////////////////////////////////////////////////////////
uint8_t var;
/////////////////////////////////////PROTOTIPOS/////////////////////////////////////////////////////////////////////////////////////////
void setup(void);
/////////////////////////////////////INTERRUPCIONES//////////////////////////////////////////////////////////////////////////////////
void __interrupt() isr(void)
{
  if (T0IF)
  {
   PORTA++;
   T0IF=0;   
   TMR0 = _tmr0_value;
  }    
  return;  
}
///////////////////////////////////////CODIGO PRINCIPAL/////////////////////////////////////////////////////////////////////
void main(void) {
    setup();
 ////LOOP PRINCIPAL/////   
    while(1)
    {
        if(!RB0)
        {
        while(!RB0);
        PORTC++;
        }    
    }
    
    
    return;
}

//////////////////////////////////////FUNCIONES (SETUP)/////////////////////////////////////////////////////////////////////
void setup(void)
{
    //configuracion de pines y puertos    
    ANSEL= 0;
    ANSELH= 0;
    TRISA= 0;
    TRISB= 0b0011;
    TRISC= 0;
    //configuracion oscilador
    OSCCONbits.IRCF = 0b0110; //4MHz
    OSCCONbits.SCS = 1;
    //configuracion timer0
    T0CS = 0;
    T0SE = 0;
    PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    //configuracion de interrupciones
    TMR0 = _tmr0_value;
    GIE= 1;
    T0IE= 1;
    T0IF= 0;
    return;   

}