// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"
#include "Leds_Lab04.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
typedef struct ButtonResult {
    uint8_t event;
} ButtonResult;

// **** Define global, module-level, or external variables here ****
ButtonResult Button1;
uint8_t buttonsState;
// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 2; // 1:64 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to CRUZID's lab4 part4 (ButtonsTest).  Compiled on %s %s.\n", __TIME__, __DATE__);

    
    printf("Please press some buttons!\n");
    
    ButtonsInit();
    LEDS_INIT();
    Button1.event = FALSE;
    
    while (1) {
        if (Button1.event) {
            uint8_t switchesState = SWITCH_STATES();
            uint8_t val = LEDS_GET();
            if (switchesState & SWITCH_STATE_SW1 && !(buttonsState & BUTTON_STATE_1) ||
                !(switchesState & SWITCH_STATE_SW1) && buttonsState & BUTTON_STATE_1) {
                val ^= 0b11; 
            }
            
            if (switchesState & SWITCH_STATE_SW2 && !(buttonsState & BUTTON_STATE_2) ||
                !(switchesState & SWITCH_STATE_SW2) && buttonsState & BUTTON_STATE_2) {
                val ^= 0b1100; 
            }
            
            if (switchesState & SWITCH_STATE_SW3 && !(buttonsState & BUTTON_STATE_3) ||
                !(switchesState & SWITCH_STATE_SW3) && buttonsState & BUTTON_STATE_3) {
                val ^= 0b110000; 
            }
            
            if (switchesState & SWITCH_STATE_SW4 && !(buttonsState & BUTTON_STATE_4) ||
                !(switchesState & SWITCH_STATE_SW4) && buttonsState & BUTTON_STATE_4) {
                val ^= 0b11000000; 
            }
            
            LEDS_SET(val);
            Button1.event = FALSE;
        }
        
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    buttonsState = ButtonsCheckEvents();
    
    if (ButtonsCheckEvents() != BUTTON_EVENT_NONE) {
        Button1.event = TRUE;
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}