
/* 
 * File: const_and_enum.h
 * Author: Ronnie Eriksson
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef general_data_header_H
#define	general_data_header_H


// PIC18F47K40 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config FEXTOSC = HS    // External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Power-up default value for COSC bits (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // Master Clear Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (Power up timer disabled)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset disabled)

// CONFIG2H
#pragma config BORV = VBOR_285  // Brown Out Reset Voltage selection bits (Brown-out Reset Voltage (VBOR) set to 2.85V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_12// WDT Period Select bits (Divider ratio 1:131072)
#pragma config WDTE = OFF       // WDT operating mode (WDT enabled regardless of sleep)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = LFINTOSC// WDT input clock selector (WDT reference clock is the 31.0 kHz LFINTOSC)

// CONFIG4L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)
#pragma config WRT4 = OFF       // Write Protection Block 3 (Block 4 (010000-013FFFh) not write-protected)
#pragma config WRT5 = OFF       // Write Protection Block 3 (Block 5 (014000-017FFFh) not write-protected)
#pragma config WRT6 = OFF       // Write Protection Block 3 (Block 6 (018000-01BFFFh) not write-protected)
#pragma config WRT7 = OFF       // Write Protection Block 3 (Block 7 (01C000-01FFFFh) not write-protected)

// CONFIG4H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config SCANE = ON       // Scanner Enable bit (Scanner module is available for use, SCANMD bit can control the module)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // UserNVM Program Memory Code Protection bit (UserNVM code protection disabled)
#pragma config CPD = OFF        // DataNVM Memory Code Protection bit (DataNVM code protection disabled)

// CONFIG5H

// CONFIG6L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR4 = OFF      // Table Read Protection Block 4 (Block 4 (010000-013FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR5 = OFF      // Table Read Protection Block 5 (Block 5 (014000-017FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR6 = OFF      // Table Read Protection Block 6 (Block 6 (018000-01BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR7 = OFF      // Table Read Protection Block 7 (Block 7 (01C000-01FFFFh) not protected from table reads executed in other blocks)

// CONFIG6H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic18f47k40.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "characters.h"

#define _XTAL_FREQ 64000000

#define RED_U LATB5
#define BLUE_U LATB4
#define GREEN_U LATB3
#define RED_L LATB2
#define BLUE_L LATB1
#define GREEN_L LATB0

#define D_CLK LATC7
#define D_STB LATC6
#define D_ENA LATC5

#define SPI_BF SSP1STATbits.BF

#define ADC_GO ADCON0bits.GO

#define START_OF_TEXT 0x02
#define END_OF_TEXT 0x03

typedef enum{
    display_U,
    display_L
}DISPLAY_PART;

typedef enum{
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
}DISPLAY_COLOR;

void __interrupt(high_priority) HiIsr(void);

void main(void);
int photoDiode(void);
void interruptSetup(void);
void ports(void);
void SPI1_Initialize(void);
void runHub75(void);
//void clockReferenceOutput(void);
void timer1Setup(void);
void setTimer1(unsigned char timeENA);
void disableTimer1(void);
void enableTimer1(void);
//void timer3Setup(void);
//void setTimer3(void);
//void disableTimer3(void);
//void enableTimer3(void);
void ADC(void);
void clkPulse(void);
void clkPulseX2(void);
//void clkPulseX4(void);
void strobe(void);
void displayENA(unsigned char timeENA);
void textColorSet(DISPLAY_PART U_L);
void backgroundColorSet(void);
void EUSART1_Initialize(void);
void UART(void);
void initialLightMeassure(void);

unsigned char colors = 0;
unsigned char lineSwitch = 0;

bool optimise = false;;

unsigned char colors;
unsigned char displayRowProperties;
char myString_1[11];
char myString_2[11];
char myString_3[11];
char myString_4[11];
uint8_t color[4];
unsigned char characterBoxLine_1[10][8], characterBoxLine_2[10][8], characterBoxLine_3[10][8], characterBoxLine_4[10][8];

int timeENA, timeENAReference;

#endif	/* XC_HEADER_TEMPLATE_H */




