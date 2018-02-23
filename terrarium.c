/*
 * File:   Blink_Main.c
 * Author: foo_f
 *
 * Created on 20 février 2018, 17:43
 */


#include <xc.h>
//#include "config_bits.h"
#include "pic16f877.h"

#define _XTAL_FREQ 4000000

#define VREF_plus  5.0f   // vref + = Vcc tension d'alimentation
#define VREF_moins 0.0f   // vref - =  0V
#define PLEINE_ECH 1024   // convertisseur 10 bits.  pleine echelle = 2^10 = 1024
#define Resolution 125    // resolution de 125 mV/C°

void Set_LEDs(int);
void Set_LED_22();
void Set_LED_26();
void Set_LED_30();
void Set_LED_34();
void Set_LED_38();


int main()
{
    TRISA0 = 1; //AN0 as Input PIN
  
    TRISC0 = 1; //RC0 as Input PIN
    TRISC1 = 1; //RC1 as Input PIN
            
    TRISC2 = 0; //RC2 as Output PIN
    TRISC3 = 0; //RC3 as Output PIN
    TRISC4 = 0; //RC4 as Output PIN
    TRISC5 = 0; //RC5 as Output PIN
    TRISC6 = 0; //RC6 as Output PIN
  
    TRISD0 = 0; //RD0 as Output PIN FAN
    TRISD1 = 0; //RD1 as Output PIN HEATER
  
    float temp = 0.0f;
    int tempREF = 26;
    int cpt = 0;
  
    unsigned long ValeurADC = 0L;
    float tension=0.0f;
    TRISA = 0xFF;  //port A  tout en entree doc p 111
    ADCON1 = 0b10001110;   // right justified vref+=Vdd vref- = Vss RA0 an autres entrées digitales  doc p 112
    ADCON0 = 0b01000001;   // Fosc/8 RA0 stop x AD on
       
    while(1)
    {
        Set_LEDs(tempREF);
        if(RC0 == 1 && RC1 == 0)   //If the Plus Button is pressed
        {
//            __delay_ms(100);    //Switch Debounce
            if(RC0 == 1 && RC1 == 0)//If the switch is still pressed
            {
                if(RC0 == 1 && tempREF != 30){
                    tempREF = tempREF + 4;
                    Set_LEDs(tempREF); 
                }
            }
        }
        if(RC1 == 1 && RC0 == 0)   //If the Minus Button is pressed
        {
//            __delay_ms(100);    //Switch Debounce
            if(RC1 == 1 && RC0 == 0)//If the switch is still pressed
            {
                if (RC1 == 1 && tempREF != 22){
                       tempREF = tempREF - 4;
                        Set_LEDs(tempREF); 
               }
            }
        }
        if(cpt ==20){
          cpt = 0;
          ADCON0bits.GO_DONE = 1;
          while(ADCON0bits.GO_DONE==1);
          ValeurADC = ADRESH <<8;
          ValeurADC += ADRESL;
          tension = (VREF_plus - VREF_moins)*ValeurADC / PLEINE_ECH;
          temp = ((tension * 1000)/Resolution)+10;

          if (temp > tempREF){
              RD1 = 0;
              RD0 = 1;
          }
          if (temp < tempREF){
              RD1=1;
              RD0 = 0;
          }
          if(temp == tempREF){
              RD1=0;
              RD0=0;
          }
          
        }
//        __delay_ms(5000);
        cpt++;
    }
}
void Set_LEDs(tempREF){
      if(tempREF == 22){
          Set_LED_22();
      }else if(tempREF == 26){
          Set_LED_26();
      }else if(tempREF == 30){
          Set_LED_30();
      }else if(tempREF == 34){
          Set_LED_34();
      }else if(tempREF == 38){
          Set_LED_38();
      } 
}
void Set_LED_22(){
        RC2 = 1;
        RC3 = 0;
        RC4 = 0;
        RC5 = 0;
        RC6 = 0;
}
void Set_LED_26(){
        RC2 = 0;
        RC3 = 1;
        RC4 = 0;
        RC5 = 0;
        RC6 = 0;
}
void Set_LED_30(){
        RC2 = 0;
        RC3 = 0;
        RC4 = 1;
        RC5 = 0;
        RC6 = 0;
}
void Set_LED_34(){
        RC2 = 0;
        RC3 = 0;
        RC4 = 0;
        RC5 = 1;
        RC6 = 0;
}
void Set_LED_38(){
        RC2 = 0;
        RC3 = 0; 
        RC4 = 0;
        RC5 = 0;
        RC6 = 1;
}