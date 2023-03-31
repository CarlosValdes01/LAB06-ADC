/* 
 * Archivo: PostLab06.c
 * Dispositivo: PIC16F887
 * Autor: Carlos Julio Valdés Oajaca
 * Compilador: XC8, MPLABX v6.05
 
 * Programa: leyendo 2 POTS en portE y desplegando en portA y displays
 * Hardware: 

 * Creado: 28 de marzo, 2023
 * Última modificación: 30 de marzo, 2023
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

///////////////////////////////////DEFINIR CONSTANTES///////////////////////////////////////////////////////////////////////////////////
#define _XTAL_FREQ 4000000 //4MHz
////////////////////////////////////VARIABLES GLOBALES/////////////////////////////////////////////////////////////////////////////////
int ADC_disp;
int ADC_volts;

uint8_t centenas;
uint8_t decenas;
uint8_t unidades;
uint8_t residuo;

uint8_t flag;
uint8_t disp_val [3];

uint8_t tabla [16] = {0b00111111,  //num 0
                      0b00000110,  //num 1
                      0b01011011,  //num 2
                      0b01001111,  //num 3
                      0b01100110,  //num 4
                      0b01101101,  //num 5
                      0b01111101,  //num 6
                      0b00000111,  //num 7
                      0b01111111,  //num 8
                      0b01100111,  //num 9
                      0b01110111,  //num A (10)
                      0b01111100,  //num B (11)
                      0b00111001,  //num C (12)
                      0b01011110,  //num D (13)
                      0b01111001,  //num E (14)
                      0b01110001};  //num F (15) 
//////////////////////////////////////////////////////////////////////////////////
uint8_t tablaCent [6] = {0b10111111,  //num 0.
                      0b10000110,  //num 1.
                      0b11011011,  //num 2.
                      0b11001111,  //num 3.
                      0b11100110,  //num 4.
                      0b11101101};  //num 5.
/////////////////////////////////////PROTOTIPOS DE FUNCIONES///////////////////////////////////////////////////////////////////////////
void setup(void);
void displays (int num);
/////////////////////////////////////INTERRUPCIONES//////////////////////////////////////////////////////////////////////////////////
void __interrupt() isr(void)
{
  if (PIR1bits.ADIF) //Interrupcion del ADC
  {
    if (ADCON0bits.CHS == 6)
        ADC_disp = ADRESH;
    else
        PORTA = ADRESH;
    PIR1bits.ADIF = 0;
  }
  if (INTCONbits.T0IF) //Interrupcion del tmr0
  {
      PORTD = 0; //limpiar banderas
      
      if (flag == 0)
      {
         PORTC = disp_val[2]; 
         PORTD = 1;
         flag = 1;
      }
      else if (flag == 1)
      {
         PORTC = disp_val[1];
         PORTD = 2;
         flag = 2; 
      }
      else if (flag == 2)
      {
         PORTC = disp_val [0];
         PORTD = 4;
         flag = 0; 
      }
      TMR0 = 131; //10ms
      INTCONbits.T0IF = 0; //reiniciar tmr0
  }
  return;
}
///////////////////////////////////////CODIGO PRINCIPAL/////////////////////////////////////////////////////////////////////
void main(void) {
    setup(); //llamar a la configuracion
    ADCON0bits.GO = 1;
    
 ////LOOP PRINCIPAL/////   
    while(1) //Loop infinito
    {
    ADC_volts = (int) (ADC_disp*((float)5/255)*(100)); //Para convertir a voltaje se agarra 
    //el valor y se divide por 255 (contador) y se multiplica por 5V. Luego, se multiplica por 100 para tener el entero y no un float.
    
    displays(ADC_volts); //convertir a centenas, decenas y unidades
    
    disp_val [0] = tabla[unidades]; //convertir numero a 7seg y guardarlo en disp_val
    disp_val [1] = tabla[decenas];
    disp_val [2] = tablaCent[centenas];
    
        if (ADCON0bits.GO == 0)
        {
            if (ADCON0bits.CHS == 6)
            
                ADCON0bits.CHS = 5;
            else 
                 ADCON0bits.CHS = 6;
            __delay_us(1000);    
            ADCON0bits.GO = 1;    
        }
    }
    return;
}

//////////////////////////////////////SETUP/////////////////////////////////////////////////////////////////////
void setup(void)
{
    //configuracion de pines y puertos    
    ANSEL= 0b01100000;  //configura los pines de los POT como analogos
    ANSELH= 0; //configura los pines de portB como digital
    
    TRISA = 0; // Configura el portA como salida
    TRISB = 0; //Configura portB como salida
    TRISE = 0b011; //Configurar entradas de los POT
    TRISC = 0;     //Salida para los displays
    TRISD = 0;     //Salida para los selectores
    
    PORTA = 0; //Inicializar los puertos en 0
    PORTB = 0;
    PORTE = 0;
    PORTC = 0;
    PORTD = 0;
    
    //configuracion oscilador
    OSCCONbits.IRCF = 0b0110; //4MHz
    OSCCONbits.SCS = 1;       //Utilizar el reloj interno
    
    //configuracion del ADC
    ADCON1bits.ADFM = 0; //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0; //Referencia en VDD
    ADCON1bits.VCFG1 = 0; //Referencia en VSS 
    
    ADCON0bits.ADCS = 0b01; //ADCS <1:0> -> 01 Fosc/8
    ADCON0bits.CHS = 5;     //CHS <3:0> AN5
    ADCON0bits.ADON = 1;    //Enceder ADC
    __delay_us(50);
    
    //configuracion tmr0
    OPTION_REGbits.T0CS = 0; //Tmr0 como temporizador
    OPTION_REGbits.PSA = 0;  //Asignar prescaler a tmr0
    OPTION_REGbits.PS = 0b010; //Prescaler de 1:8
    TMR0 = 131; //1ms
    
    //configuracion de interrupciones
    INTCONbits.TMR0IE = 1; //habilitar interrupcion del tmr0
    INTCONbits.T0IF = 0; //inicializar bandera de tmr0 en 0
    PIR1bits.ADIF = 0;   //Inicializar bandera en 0
    PIE1bits.ADIE = 1;   //Habilitar interrupciones del ADC
    INTCONbits.PEIE = 1; //Habilitar interrupciones perifericas
    INTCONbits.GIE = 1; // Habilita interrupciones generales

    //return;   
}
//////////////////////////////////////FUNCIONES/////////////////////////////////////////////////////////////////////
void displays(int num){
    
    centenas = (uint8_t)(num/100);
    residuo = (num%100);
    
    decenas = (residuo/10);
    residuo = (residuo%10);
    
    unidades = (residuo);
            
    return;
}