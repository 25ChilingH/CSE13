// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab04.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
typedef struct Timer {
    uint8_t event;
} Timer;

enum Led {
    LED1 = 0b1,
    LED2 = 0b10,
    LED3 = 0b100,
    LED4 = 0b1000,
    LED5 = 0b10000,
    LED6 = 0b100000,
    LED7 = 0b1000000,
    LED8 = 0b10000000, 
};

// **** Define global, module-level, or external variables here ****

Timer Timer1;

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
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
    printf("Welcome to CRUZID's lab4 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);

    LEDS_INIT();
    ButtonsInit();
    while (1) {
        if (Timer1.event) {
            char val = LEDS_GET();
            uint8_t switchesState = SWITCH_STATES();
            uint8_t buttonsState = BUTTON_STATES();
            // based on the state of the switches and what happened in the event we toggle the led on
            // or off with a bitwise xor. We do this for every switch and its respective button
            if (switchesState & SWITCH_STATE_SW1 && buttonsState & BUTTON_EVENT_1UP || !(switchesState & SWITCH_STATE_SW1) && buttonsState & BUTTON_EVENT_1DOWN) {
                val ^= LED1 ^ LED2;
            }
            else if (switchesState & SWITCH_STATE_SW2 && buttonsState & BUTTON_EVENT_2UP || !(switchesState & SWITCH_STATE_SW2) && buttonsState & BUTTON_EVENT_2DOWN) {
                val ^= LED3 ^ LED4;
            }
            else if (switchesState & SWITCH_STATE_SW3 && buttonsState & BUTTON_EVENT_4UP || !(switchesState & SWITCH_STATE_SW3) && buttonsState & BUTTON_EVENT_3DOWN) {
                val ^= LED5 ^ LED6;
            }
            else if (switchesState & SWITCH_STATE_SW4 && buttonsState & BUTTON_EVENT_4UP || !(switchesState & SWITCH_STATE_SW4) && buttonsState & BUTTON_EVENT_4DOWN) {
                val ^= LED7 ^ LED8;
            }
            
            // we set the led to the result of all the if statements
            LEDS_SET(val);
            // reset the timer event to none
            Timer1.event = FALSE;
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
    Timer1.event = (ButtonsCheckEvents() != BUTTON_EVENT_NONE) ? TRUE : FALSE;
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}