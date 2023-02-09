//SLAVE 1


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

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "mapf.h"
#include "SPI.h"
#include "setupADC.h"
#include "oscilador.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000

uint8_t check = 0;
uint8_t lecADC;
uint8_t check;

uint8_t contador;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
    //INTERRUPCION DEL SPI
    //if (PIR1bits.SSPIF){
    if(PIR1bits.SSPIF){
        check = SSPBUF;  //cargamos el valor clean para limpiar
        __delay_ms(5);
        if (check == 1){
            SSPBUF = contador;
            //PORTD = contador;
        }
        else if (check == 0){
            lecADC = ADC_read(0);
            SSPBUF = lecADC;
            //PORTD = lecADC;
        }
        PIR1bits.SSPIF = 0;
        //SSPIF = 0;
    }
    
    //LECTURA DE LOS BOTONES
	if (INTCONbits.RBIF == 1){
        //check = spiRead();
    if (PORTBbits.RB0 == 0)
    {
        __delay_ms(3);
        if (PORTBbits.RB0 == 1){ //incremento el puerto
            contador++;
            PORTD = contador;
            INTCONbits.RBIF = 0;
        }
    }
    else if (PORTBbits.RB1 == 0){
        __delay_ms(3);
        if (PORTBbits.RB1 == 1){
            contador--; 
            PORTD = contador;
            INTCONbits.RBIF = 0;
		}
    }
    
    INTCONbits.RBIF = 0;
    } 


}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    ADC_config(0x01);
    setupINTOSC(6);
    lecADC = 0;
    check = 0;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        ;
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    
    TRISB = 0b00000011;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
    
    //Interrupciones
    INTCONbits.RBIE = 1; 
    INTCONbits.RBIF = 0;
    INTCONbits.GIE = 1; //interrupciones globales
    
    WPUBbits.WPUB0 = 1; //inputs
    WPUBbits.WPUB1 = 1;
    IOCBbits.IOCB0 = 1; //inputs
    IOCBbits.IOCB1 = 1;
    
    OPTION_REGbits.nRBPU = 0; //no RBPU, habilitan los pullups internos
    
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}