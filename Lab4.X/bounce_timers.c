// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab04.h"


// **** Declare any datatypes here ****
#define TIMERA_COUNTDOWN 2*38;
#define TIMERB_COUNTDOWN 3*38;
#define TIMERC_COUNTDOWN 5*38;

typedef struct Timer {
    uint8_t event;
    int16_t timeRemaining;
} Timer;

enum Led {
    LED1 = 0b1,
    LED2 = 0b10,
    LED3 = 0b100
};

// **** Define global, module-level, or external variables here ****
Timer TimerA;
Timer TimerB;
Timer TimerC;

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
    printf("Welcome to CRUZID's lab4 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);
    
    LEDS_INIT();
    TimerA.event = FALSE;
    TimerB.event = FALSE;
    TimerC.event = FALSE;
    
    TimerA.timeRemaining = TIMERA_COUNTDOWN;
    TimerB.timeRemaining = TIMERB_COUNTDOWN;
    TimerC.timeRemaining = TIMERC_COUNTDOWN;
    
    char val = 0x00;
    
    while (1) {
        if (TimerA.event) {
            printf("A ");
            TimerA.event = FALSE;
            val ^= LED1;
        }
        if (TimerB.event) {
            printf("B ");
            TimerB.event = FALSE;
            val ^= LED2;
        }
        if (TimerB.event) {
            printf("C ");
            TimerB.event = FALSE;
            val ^= LED3;
        }
        LEDS_SET(val);
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
    
    TimerA.timeRemaining--;
    TimerB.timeRemaining--;
    TimerC.timeRemaining--;
    
    if (TimerA.timeRemaining <= 0) {
        TimerA.event = TRUE;
        TimerA.timeRemaining = TIMERA_COUNTDOWN;
    }
        
    if (TimerB.timeRemaining <= 0) {
        TimerB.event = TRUE;
        TimerB.timeRemaining = TIMERB_COUNTDOWN;
    }
        
    if (TimerC.timeRemaining <= 0) {
        TimerC.event = TRUE;
        TimerC.timeRemaining = TIMERC_COUNTDOWN;
    }
    
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}