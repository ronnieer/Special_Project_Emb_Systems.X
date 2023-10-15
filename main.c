/*
 * File:   main.c
 * Author: Ronnie
 *
 * Created on den 22 juli 2023, 13:09
 */

#include "general_data_header.h"

/*
################################################################################
###################  MAIN PROGRAM  #############################################
################################################################################   
*/

void main(void){
    ports();
    EUSART1_Initialize();
    UART();
    SPI1_Initialize();
    interruptSetup();
    //clockReferenceOutput();
    timer1Setup();
    //timer3Setup();
    ADC();
    initialLightMeassure();
    runHub75();
}

/*
================================================================================
===================  ADC ROTINE  ===============================================
================================================================================ 
*/

int photoDiode(void){
    unsigned char photoDiodeADCResult;
    ADC_GO = 1;
    while (ADC_GO == 1) 
    {
        NOP();
    }
    photoDiodeADCResult = ADRESH;
    return photoDiodeADCResult;
}

/*
================================================================================
===================  HUB75 LOW LEVEL FUNCTIONS  ================================
================================================================================ 
*/

void clkPulse(void){
    D_CLK = 1; 
    D_CLK = 0;       
}

void clkPulseX2(void){
    clkPulse();
    clkPulse();
}

//void clkPulseX4(void){
//    clkPulse();
//    clkPulse();
//    clkPulse();
//    clkPulse();
//}
    
void strobe(void){
    D_STB = 1; 
    D_STB = 0;
}


void displayENA(unsigned char timeENA){
    if(!T3CONbits.ON){
        D_ENA = 0;
        setTimer1(timeENA);
        while(!PIR4bits.TMR1IF) NOP();
        D_ENA = 1;  
    }
}

void textColorSet(DISPLAY_PART U_L){
    
    bool red, green, blue;
    //colors = (unsigned char)(WHITE << 4);
    red = (((colors >> 4) & 0x01) == 0x01);
    green = (((colors >> 4) & 0x02) == 0x02);
    blue = (((colors >> 4) & 0x04) == 0x04);
    
    if(U_L == display_U){
        RED_U = red;
        GREEN_U = green;
        BLUE_U = blue;

        RED_L = false;
        GREEN_L = false;
        BLUE_L = false;
        }
    else if (U_L == display_L){
        RED_U = false;
        GREEN_U = false;
        BLUE_U = false;

        RED_L = red;
        GREEN_L = green;
        BLUE_L = blue;        
    }
}

void backgroundColorSet(void){
    RED_U = false;
    GREEN_U = false;
    BLUE_U = false;

    RED_L = false;
    GREEN_L = false;
    BLUE_L = false;
}

/*
================================================================================
===================  INTERRUPT SERVICE ROTINE  =================================
================================================================================ 
*/

void __interrupt(high_priority) hiISR(void){
    
    float photoDiodeResult;
    int timeENAMeasure;
    char endOfTextChar = 0;
    
    if(PIR3bits.SSP1IF){

        disableTimer1();
        D_ENA = 1;

/*
--------------------------------------------------------------------------------
-------------------  LOOK FOR ASCII START OF TEXT  -----------------------------
-------------------------------------------------------------------------------- 
*/  
        
//        while(SSP1BUF != 0x02){
//            while(!SPI_BF){}
//        }
        //setTimer3();
        while(true){
            while(!SPI_BF){}
            unsigned char dummy = SSP1BUF;
            if(dummy == START_OF_TEXT) break;
            //else if(SSP1BUF == END_OF_TEXT) goto QUIT;
            //else if(PIR4bits.TMR3IF) goto QUIT;
        }
        
/*
--------------------------------------------------------------------------------
-------------------  READ REQUESTED TEXT COLOR  --------------------------------
-------------------------------------------------------------------------------- 
*/          
        
    RESTART:
        while(!SPI_BF){}
        colors = SSP1BUF;
        if(colors == START_OF_TEXT) goto RESTART;
        else if (colors == END_OF_TEXT) goto QUIT;
        
/*
--------------------------------------------------------------------------------
-------------------  READ LED MATRIX LINE 1 TEXT MESSAGE  ----------------------
-------------------------------------------------------------------------------- 
*/        
        
        for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){

            while(!SPI_BF){}
            myString_1[characterIndex] = SSP1BUF;
            if(myString_1[characterIndex] == START_OF_TEXT) goto RESTART;
            else if (myString_1[characterIndex] == END_OF_TEXT) goto QUIT;
        }      
        
/*
--------------------------------------------------------------------------------
-------------------  READ LED MATRIX LINE 2 TEXT MESSAGE  ----------------------
-------------------------------------------------------------------------------- 
*/                
        
        for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
  
            while(!SPI_BF){}
            myString_2[characterIndex] = SSP1BUF;
            if(myString_2[characterIndex]== START_OF_TEXT) goto RESTART;
            else if (myString_2[characterIndex] == END_OF_TEXT) goto QUIT;            
        }     
        
/*
--------------------------------------------------------------------------------
-------------------  READ LED MATRIX LINE 3 TEXT MESSAGE  ----------------------
-------------------------------------------------------------------------------- 
*/                
        
        for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){

            while(!SPI_BF){}
            myString_3[characterIndex] = SSP1BUF;
            if(myString_3[characterIndex] == START_OF_TEXT) goto RESTART;
            else if (myString_3[characterIndex] == END_OF_TEXT) goto QUIT;            
        }
        
/*
--------------------------------------------------------------------------------
-------------------  READ LED MATRIX LINE 4 TEXT MESSAGE  ----------------------
-------------------------------------------------------------------------------- 
*/                

        for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){

            while(!SPI_BF){}
            myString_4[characterIndex] = SSP1BUF;
            if(myString_4[characterIndex] == START_OF_TEXT) goto RESTART;
            else if (myString_4[characterIndex] == END_OF_TEXT) goto QUIT;            
        }  
        
/*
--------------------------------------------------------------------------------
-------------------  LOOK FOR ASCII END OF TEXT  -------------------------------
-------------------------------------------------------------------------------- 
*/         
        
        while(!SPI_BF){}
        endOfTextChar = SSP1BUF;
        if(endOfTextChar == START_OF_TEXT) goto RESTART;

/*
--------------------------------------------------------------------------------
-------------------  IF END OF TEXT RETRIEVED,                     -------------
-------------------  NULL TERMINATE TEXT ARRAYS TO BECOME STRINGS  -------------
-------------------------------------------------------------------------------- 
*/          
          
        timeENA = timeENAReference;
        
        if(endOfTextChar == END_OF_TEXT){
            myString_1[10] = '\0';
            myString_2[10] = '\0';
            myString_3[10] = '\0';
            myString_4[10] = '\0';
            
/*
--------------------------------------------------------------------------------
-------------------  PACK STRINGS INTO "BOXES" FOR DISPLAY WRITE  --------------
-------------------------------------------------------------------------------- 
*/              

            for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
                for(unsigned char characterLineIndex = 0; characterLineIndex < 8; characterLineIndex++){
                    char myCharacter = myString_1[characterIndex];
                    characterBoxLine_1[characterIndex][characterLineIndex] = myCharacters[myCharacter][characterLineIndex];
                }
            }
            for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
                for(unsigned char characterLineIndex = 0; characterLineIndex < 8; characterLineIndex++){
                    char myCharacter = myString_2[characterIndex];
                    characterBoxLine_2[characterIndex][characterLineIndex] = myCharacters[myCharacter][characterLineIndex];
                }
            }    
            for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
                for(unsigned char characterLineIndex = 0; characterLineIndex < 8; characterLineIndex++){
                    char myCharacter = myString_3[characterIndex];
                    characterBoxLine_3[characterIndex][characterLineIndex] = myCharacters[myCharacter][characterLineIndex];
                }
            }  
            for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
                for(unsigned char characterLineIndex = 0; characterLineIndex < 8; characterLineIndex++){
                    char myCharacter = myString_4[characterIndex];
                    characterBoxLine_4[characterIndex][characterLineIndex] = myCharacters[myCharacter][characterLineIndex];
                }
            }
        }
    }
    
    QUIT:

    
/*
--------------------------------------------------------------------------------
-------------------  READ PHOTODIODE FOR AMBIENT LIGHT  ------------------------
-------------------------------------------------------------------------------- 
*/            
        
        photoDiodeResult = photoDiode();
        
/*
--------------------------------------------------------------------------------
-------------------  ADJUST PHOTODIODE CURVE FOR BETTER DISPLAY RESPONSE  ------
-------------------------------------------------------------------------------- 
*/            
        
        timeENAMeasure = (int)(0.25 * ((sqrt(photoDiodeResult + 10)) + 2 * photoDiodeResult));
        if(timeENAMeasure <= 0) timeENAMeasure = 1;
        else if(timeENAMeasure >= 110) timeENAMeasure = 110;

/*
--------------------------------------------------------------------------------
-------------------  OPTIMIZE DISPLAY BRIGHTNES RESPONSE -  --------------------
-------------------  IF RESPONSE BETTER THAN TWO UNITS,     --------------------
-------------------  THEN STOP OPTIMIZING                   --------------------
-------------------------------------------------------------------------------- 
*/  
        
        if(optimise && abs(abs(timeENAReference) - abs(timeENAMeasure)) <= 2){
            timeENAReference = timeENAMeasure;
            optimise = false;
        }        
        else if((timeENAReference - timeENAMeasure) > 2){
            timeENAReference -= 2;
            optimise = true;
        }
        else if((timeENAMeasure - timeENAReference) > 2){
            timeENAReference += 2;
            optimise = true;
        }    
    
    
    enableTimer1();
    
    PIR0 = 0x00;
    PIR1 = 0x00;
    PIR2 = 0x00;
    PIR3 = 0x00;
    PIR5 = 0x00;
    PIR5 = 0x00;
    PIR6 = 0x00;
    PIR7 = 0x00;
}


/*
================================================================================
===================  HUB75 MAIN FUNCTION  ======================================
================================================================================ 
*/

void runHub75(void){
    
    DISPLAY_PART upperLowerDisplay;
    char myString_1[11];
    char myString_2[11];
    char myString_3[11];
    char myString_4[11];

    while(true){
        
/*
--------------------------------------------------------------------------------
-------------------  DISPLAY ROW 1 MAIN FUNCTION  ------------------------------
-------------------------------------------------------------------------------- 
*/

        for(unsigned char characterLineIndex = 0; characterLineIndex < 8; characterLineIndex++){
            clkPulseX2();
            for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
                for(unsigned char characterLinePixelsIndex = 0; characterLinePixelsIndex < 6; characterLinePixelsIndex++){
                    LATD = (unsigned char)(characterLineIndex << 4);  
                    unsigned char lineOfPixels = characterBoxLine_1[characterIndex][characterLineIndex];

                    if(((lineOfPixels >> characterLinePixelsIndex) & 0x01) == 0x01){
                        textColorSet(display_U);
                    }
                    else{
                        backgroundColorSet();      
                    }
                    clkPulse();
                }            
            }
            backgroundColorSet();
            clkPulseX2();
            strobe();

/*
-------------------  DISPLAY ENABLE TIME DEPENDING ON PHOTO DIODE  -------------
*/
            
            displayENA((unsigned char)timeENA);
        }
        
/*
--------------------------------------------------------------------------------
-------------------  DISPLAY ROW 2 MAIN FUNCTION  ------------------------------
-------------------------------------------------------------------------------- 
*/        
        
        for(unsigned char characterLineIndex = 0; characterLineIndex < 8; characterLineIndex++){
            clkPulseX2();
            for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
                for(unsigned char characterLinePixelsIndex = 0; characterLinePixelsIndex < 6; characterLinePixelsIndex++){
                    LATD = (unsigned char)((characterLineIndex + 8) << 4);    
                    unsigned char lineOfPixels = characterBoxLine_2[characterIndex][characterLineIndex];
                    if(((lineOfPixels >> characterLinePixelsIndex) & 0x01) == 0x01){
                        textColorSet(display_U);
                    }
                    else{
                        backgroundColorSet();    
                    }                        
                    clkPulse();
                }            
            }
            backgroundColorSet();
            clkPulseX2();
            strobe();
            
/*
-------------------  DISPLAY ENABLE TIME DEPENDING ON PHOTO DIODE  -------------
*/            

            displayENA((unsigned char)timeENA);
        }
        
/*
--------------------------------------------------------------------------------
-------------------  DISPLAY ROW 3 MAIN FUNCTION  ------------------------------
-------------------------------------------------------------------------------- 
*/        
        
        for(unsigned char characterLineIndex = 0; characterLineIndex < 8; characterLineIndex++){
            clkPulseX2();
            for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
                for(unsigned char characterLinePixelsIndex = 0; characterLinePixelsIndex < 6; characterLinePixelsIndex++){
                    LATD = (unsigned char)(characterLineIndex << 4);
                    unsigned char lineOfPixels = characterBoxLine_3[characterIndex][characterLineIndex];
                    if(((lineOfPixels >> characterLinePixelsIndex) & 0x01) == 0x01){
                        textColorSet(display_L);
                    }
                    else{
                        backgroundColorSet();    
                    }                        
                    clkPulse();
                }            
            }
            backgroundColorSet();
            clkPulseX2();
            strobe();

/*
-------------------  DISPLAY ENABLE TIME DEPENDING ON PHOTO DIODE  -------------
*/            
            
            displayENA((unsigned char)timeENA);
        }
        
/*
--------------------------------------------------------------------------------
-------------------  DISPLAY ROW 4 MAIN FUNCTION  ------------------------------
-------------------------------------------------------------------------------- 
*/        
        
        for(unsigned char characterLineIndex = 0; characterLineIndex < 8; characterLineIndex++){
            clkPulseX2();
            for(unsigned char characterIndex = 0; characterIndex < 10; characterIndex++){
                for(unsigned char characterLinePixelsIndex = 0; characterLinePixelsIndex < 6; characterLinePixelsIndex++){
                    LATD = (unsigned char)((characterLineIndex + 8) << 4);
                    unsigned char lineOfPixels = characterBoxLine_4[characterIndex][characterLineIndex];
                    if(((lineOfPixels >> characterLinePixelsIndex) & 0x01) == 0x01){
                        textColorSet(display_L);
                    }
                    else{
                        backgroundColorSet();    
                    }                        
                    clkPulse();
                }            
            }
            backgroundColorSet();
            clkPulseX2();
            strobe();
            
/*
-------------------  DISPLAY ENABLE TIME DEPENDING ON PHOTO DIODE  -------------
*/            

            displayENA((unsigned char)timeENA);
        }
    }
}
