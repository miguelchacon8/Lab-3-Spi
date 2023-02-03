/*
 * File:   main.c
 * Author: Miguel Chacón
 * Programa: LAB 02 4 bits
 * Fecha de creación: 25/01/2023
 */

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

#include "LCD.h"
#include "mapf.h"
#include "SPI.h"
#include "oscilador.h"
#include "setupADC.h"

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define _XTAL_FREQ 4000000 //4MHz
void setup(void);
void valorLCD(void);
void calculovolt(void);


char voltaje1[3];
char voltaje2[3];
char contador[3];
uint8_t V1 = 0;
uint8_t V2 = 0;

unsigned int VOLT1 = 0;


unsigned int u1 = 0;
unsigned int d1 = 0;

unsigned int u2 = 0;
unsigned int d2 = 0;
unsigned int cont = 0;
//VARIABLES PARA LCD
char buffer[20];

//******************************************************************************
// Código Principal
//******************************************************************************
void main(void) {
    setupINTOSC(6);
    setup();
    Lcd_Init();// Inicializa la pantalla LCD
    __delay_us(100);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("S1:    S2:   S3:");

    
    while(1){
        setup();
        PORTCbits.RC1 = 0;       //Slave Select
        __delay_ms(1);  
        spiWrite("A");
        V1 = spiRead();
        VOLT1 = map(V1, 0, 255, 1, 200); //se mapea el valor 
        calculovolt();
        valorLCD();
        
        spiWrite("B");
        cont = spiRead();

        __delay_ms(1);
        PORTCbits.RC1 = 1;       //Slave Deselect 


    }
}
//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************
void setup(void){
    //PUERTOS
    ANSELH = 0;
    TRISB = 0b00000011;
    TRISA = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;

    TRISD = 0;
    
    PORTA = 0; //inicializar puertos
    PORTB = 0;
    //PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    PORTCbits.RC1 = 1; //S 1
    //PORTCbits.RC2 = 1; // S 2
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    __delay_ms(10);
}

//  FUNCION PARA OBTENER VALOR EN LCD
void valorLCD(void){
    Lcd_Set_Cursor(2,1);
    sprintf(buffer, "%u", u1); //ESCRIBO unidad V1
    Lcd_Write_String(buffer);

    Lcd_Set_Cursor(2,2);
    Lcd_Write_String(".");
    
    Lcd_Set_Cursor(2,3);
    sprintf(buffer, "%u", d1);  //ESCRIBO decena V1
    Lcd_Write_String(buffer);
    
    Lcd_Set_Cursor(2,7);
    sprintf(buffer, "%u", cont);  //escribo contador
    Lcd_Write_String(buffer);
    
}//******************************************************************************
// Función para calcular valores de unidades y decenas:
//******************************************************************************
void calculovolt(void){
    u1 = VOLT1/40; 
    d1 = VOLT1%40;
    //u2 = VOLT2/40;
    //d2 = VOLT2%40;
}

//