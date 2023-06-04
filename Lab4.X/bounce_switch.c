
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
					


//CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab04.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

#define LEFT 0
#define RIGHT 1

// **** Declare any datatypes here ****
typedef struct Timer {
    uint16_t timeRemaining;
    uint8_t event;
} Timer;


// **** Define global, module-level, or external variables here ****
static Timer Timer1;												 

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
    printf("Welcome to CRUZID's lab4 part2 (bounce_switch).  Compiled on %s %s.\n",__TIME__,__DATE__);
    LEDS_INIT();
    Timer1.event = FALSE;
    
    uint8_t current_dir = RIGHT;
							 
	while(1) {
        if (Timer1.event) {
            char leds = (LEDS_GET() == 0x00) ? 0x01 : LEDS_GET();
            if (leds & 0xFF || leds & 0x01) {
                current_dir ^= 1;
            }
            
            if (current_dir == LEFT) {
                leds <<= 1;
            }
            else {
                leds >>= 1;
            }
            
            LEDS_SET(leds);
            Timer1.event = FALSE;
        }
    }			


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}


/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    if (Timer1.timeRemaining <= 0) {
        Timer1.event = TRUE;
        
        uint16_t countdown = 38*5;
        uint8_t switchesState = SWITCH_STATES();
        if (switchesState & SWITCH_STATE_SW4) {
            countdown -= 38;
        }
        if (switchesState & SWITCH_STATE_SW3) {
            countdown -= 38;
        }
        if (switchesState & SWITCH_STATE_SW2) {
            countdown -= 38;
        }
        if (switchesState & SWITCH_STATE_SW1) {
            countdown -= 38;
        }
        
        Timer1.timeRemaining = countdown;
    }
    
    Timer1.timeRemaining--;
    

    
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/									
	 
}