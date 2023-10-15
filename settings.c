/*
 * File:   settings.c
 * Author: Ronnie
 *
 * Created on den 14 augusti 2023, 07:10
 */

#include "general_data_header.h"

/*
################################################################################
###################  PROGRAM SETTINGS  #########################################
################################################################################   
*/

/*
================================================================================
===================  PORTS SETUP  ==============================================
================================================================================ 
*/

void ports(void)
{
    ANSELA = 0x00;
    TRISA = 0x00;
    
    ANSELB = 0x00;
    TRISB = 0x00;
    
    ANSELC = 0x00;
    TRISC = 0x07;
    
    ANSELD = 0x01;
    TRISD = 0x01;
    
    ANSELE = 0x00;
    TRISE = 0x00;
    
    SLRCONA = 0x00;                                                             // LOW SLEW RATE
    SLRCONB = 0x00; //0xFF;
    SLRCONC = 0x00;//FF;
    SLRCOND = 0x00;
    SLRCONE = 0x00; //0xFF;
    
    WPUA = 0x00;                                                                // WEAK PULLUP DISABLED
    WPUB = 0x00;                                                                
    WPUC = 0x00;
    WPUD = 0x00;    
    WPUE = 0x00;  
}

/*
================================================================================
===================  INTERRUPT SETUP  ==========================================
================================================================================ 
*/

void interruptSetup(void){
    INTCON = 0xC0;
    PIR3bits.SSP1IF = 0;
    PIE3bits.SSP1IE = 1;
//    IPR3bits.RC1IP = 1;
//    IPR3bits.BCL1IP = 1;
    PIR4bits.TMR1IF = 0;
}

/*
================================================================================
===================  TIMER 1 SETUP  ============================================
================================================================================ 
*/

void timer1Setup(void)
{
    T1CONbits.ON = 0;
    T1CONbits.CKPS = 0b00;                                                      // PRESCALE 1:1
    T1CONbits.NOT_SYNC = 0;
    T1CONbits.RD16 = 1;
    T1CLKbits.CS = 0b0101;                                                      // LF INT OSC 500 kHz
}
 
void setTimer1(unsigned char timeENA)
{
    T1CONbits.ON = 0;
    TMR1H = 0xFF;
    TMR1L = 0xFF - timeENA;                                                      
    PIR4bits.TMR1IF = 0;
    T1CONbits.ON = 1;
}

void disableTimer1(void)
{
    T1CONbits.ON = 0;
}

void enableTimer1(void)
{
    T1CONbits.ON = 1;
}

/*
================================================================================
===================  TIMER 3 SETUP  ============================================
================================================================================ 
*/

//void timer3Setup(void)
//{
//    T3CONbits.ON = 0;
//    T3CONbits.CKPS = 0b00;                                                      // PRESCALE 1:1
//    T3CONbits.NOT_SYNC = 0;
//    T3CONbits.RD16 = 1;
//    T3CLKbits.CS = 0b0101;                                                      // LF INT OSC 500 kHz
//}
// 
//void setTimer3(void)
//{
//    T3CONbits.ON = 0;
//    TMR3H = 0xFF;
//    TMR3L = 0xFF - 50;                                                      
//    PIR4bits.TMR3IF = 0;
//    T3CONbits.ON = 1;
//}
//
//void disableTimer3(void)
//{
//    T3CONbits.ON = 0;
//}
//
//void enableTimer3(void)
//{
//    T3CONbits.ON = 1;
//}

/*
================================================================================
===================  SPI SLAVE SETUP  ==========================================
================================================================================ 
*/

void SPI1_Initialize(void)
{
    //Setup PPS PinsS
    
    SSP1CLKPPS = 0x10;
    SSP1DATPPS = 0x11;
    SSP1SSPPS = 0x12;    

    // SPI setup
    SSP1STAT = 0x00;
    SSP1CON1 = 0x25;
    SSP1CON3bits.BOEN = 1;
    SSP1CON1bits.SSPEN = 1;
    PMD4bits.MSSP1MD = 0;
}
    
/*
================================================================================
===================  ADC FUNCTION SETUP  =======================================
================================================================================ 
*/

void ADC(void)
{
    ADCLK = 0b00001000;                                                         // FOSC/16
    ADREF = 0b00000000;                                                         // VREF- TO VREF-// VREF+ TO VREF 4,096 V    
    //FVRCON = 0b11011111;                                                        // FIXED VOLTAGE REFFERENCE CONTROL REGISTER    
    ADPCH = 0b00011000;                                                         // ANA1            
    ADCON0 = 0b10000000;                                                        // (304) A/ D CONVERTER CONTROL REG 0, 7 NOT IN USE, 6 - 2 AN1, 1 GO/ DONE, 0 ADC ON            
    ADCON1 = 0b00000000;                                                        //
    ADCON2 = 0b00100100;                                                        // LOWPASS MODE ADC
    ADCON3 = 0b00001000;                                                        // NEWER INTERRUPT
    PMD2 = 0b01000111;                                                          // ADC MODULE ENA      
    ADACQ = 0x0F;                                                               // ADC TIME CONTROL
    ADRESH = 0x00;                                                              // (307) ADC HIGH OUTPUT
    ADRESL = 0x00;                                                              // (307) ADC LOW OUTPUT
}

/*
================================================================================
===================  RS232 FUNCTION SETUP  =====================================
================================================================================ 
*/

void EUSART1_Initialize(void)
{
    //RC6PPS = 0x09;
    RC3PPS = 0x09;
    RX1PPS = 0x17;
    TX1STA = 0x24; //0x04; //0x24; //0x2E;                                                              // 1 - CLOCK SOR BIT DONT CARE// 0 - 8 BIT TRANSM// 1 - TRANSM ENA// 0 - ASYNC MOD// 1 - SEND SYNC BRAK ENA CLEARED BY H/W// 1 - HIGH SPEED BAUDC/4// 1 - TRANSM SHIFT STAREG STA BIT EMTY// 0 PARITY BIT 9
    //RC1STA = 0x90; //0x80;                                                              // 1 - SERIALPORT ENA// 0 - 8 BIT RECEPTION// 0 - DON'T CARE// 1 - ENA RECIEVER// 0 - DISA ADRESS DET// 0 - FRAMING ERR DISA// 0 - NO OVERRUN ERR// 0 - 9TH BIR DATA DISA
    BAUD1CON = 0x08; //0x48; 
    SP1BRGL = 0xA0;
    SP1BRGH = 0x01; 
}

/*
================================================================================
===================  UART BUFFER  ==============================================
================================================================================ 
*/

void UART(void)
{
    unsigned char buffer[256];    
}

/*
================================================================================
===================  CLOCK REFFERERENCE OUTPUT SETUP  ==========================
================================================================================ 
*/

//void clockReferenceOutput(void){
//    CLKRCON = 0x92 ;
//    CLKRCLK = 0x00;
//    RC4PPS = 0x14;
//}

/*
================================================================================
===================  INITIAL LIGHT MEASUREMENT  ================================
================================================================================ 
*/

void initialLightMeassure(void){
    timeENAReference = photoDiode();
}

