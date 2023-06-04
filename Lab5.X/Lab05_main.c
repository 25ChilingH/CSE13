// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CSE13E Support Library
#include "BOARD.h"
#include "Adc.h"
#include "Buttons.h"
#include "Leds.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>



// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define BAKE_TEMP 350
#define BAKE_TIME 1
#define BROIL_TEMP 500
#define SECOND 5


// **** Set any local typedefs here ****
typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING
} OvenState;

typedef enum {
    BAKE, TOAST, BROIL
} CookingMode;

typedef enum {
    TIME, TEMPERATURE
} Selector;

typedef struct {
    OvenState state;
    uint16_t temperature, initTime, remainingTime;
    Selector selector;
    CookingMode mode;
    //add more members to this struct
} OvenData;

typedef struct Timer{
    uint8_t buttonEvent, adcEvent;
} Timer;

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****
static OvenData toaster;
static int startTimer, freeRunningTimer = 0;
static uint8_t TIMER_TICK = FALSE;
static Timer Timer1;
static const uint8_t LED_ACTIVATION[9] = {0b0, 0b1, 0b11, 0b111, 0b1111, 0b11111, 0b111111, 0b1111111, 0b11111111};
// **** Put any helper functions here ****
void setLEDs(void)
{
    if (toaster.remainingTime > 0) {
        float temp = ((float) toaster.remainingTime) / toaster.initTime * 9;
        LEDS_SET(LEDActivation[temp]);
    }
    else {
        toaster.state = SETUP;
        LEDS_SET(LEDActivation[0]);
    }
    
}

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData){
    char s[100];
    const char* ovenTopsOn[7] = {"|", OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, "|"};
    const char* ovenTopsOff[7] = {"|", OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, "|"};

    const char* ovenBottomsOn[7] = {"|", OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, "|"};
    const char* ovenBottomsOff[7] = {"|", OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, "|"};

    
    switch (toaster.mode) {
        case BAKE:
            printf("Mode: Bake\n");
            if (toaster.state == COOKING) {
                printf("%s\n|     |\n", ovenTopsOff);
                if (toaster.selector == TEMPERATURE) {
                    printf("|     |  Time: %ds\n", toaster.initTime);
                    printf("|     |  >Temperature: %dF\n", toaster.temperature);
                }
                else {
                    printf("|     |  >Time: %ds\n", toaster.initTime);
                    printf("|     |  Temperature: %dF\n", toaster.temperature);
                }
                printf("%s\n", ovenBottomsOff);  
            }
            
            else {
                printf("%s\n|     |\n", ovenTopsOn);
                printf("|     |  Time Left: %ds\n", toaster.remainingTime);
                printf("|     |  Temperature: %dF\n", toaster.temperature);
                printf("%s\n", ovenBottomsOn); 
            }
            break;
        case TOAST:
            printf("Mode: Toast\n");
            printf("%s\n|     |\n", ovenTopsOff);
            if (toaster.state == COOKING) {
                printf("|     |  Time: %ds\n", toaster.initTime);
                printf("|     |\n");
                printf("%s\n", ovenBottomsOff); 
            }  
            else {
                printf("|     |  Time Left: %ds\n", toaster.remainingTime);
                printf("|     |\n");
                printf("%s\n", ovenBottomsOn); 
            }
            break;
        case BROIL:
            printf("Mode: Broil\n");
            if (toaster.state == COOKING) {
                printf("%s\n|     |\n", ovenTopsOff);
                printf("|     |  Time: %ds\n", toaster.initTime);
                printf("|     |  Temperature: %dF\n", toaster.temperature);
            }  
            else {
                printf("%s\n|     |\n", ovenTopsOn);
                printf("|     |  Time Left: %ds\n", toaster.remainingTime);
                printf("|     |  Temperature: %dF\n", toaster.temperature);
            }
            printf("%s\n", ovenBottomsOff);
            break;
    }
    //update OLED here
    OledClear(OLED_COLOR_BLACK);
    OledDrawString(s);
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    switch (toaster.state) {
        case SETUP:
            if (Timer1.adcEvent) {
                uint16_t val = AdcRead();
                val = (val & 0xff00) >> 8;
                if (toaster.selector == TIME) {
                    toaster.initTime = val + 1;
                    toaster.remainingTime = toaster.initTime;
                }
                else if (toaster.selector == TEMPERATURE) toaster.temperature = val + 300;
                updateOvenOLED(toaster);
            }
            if (Timer1.buttonEvent & BUTTON_EVENT_3DOWN) {
                startTimer = freeRunningTimer;
                toaster.state = SELECTOR_CHANGE_PENDING;
            }
            if (Timer1.buttonEvent & BUTTON_EVENT_4DOWN) {
                startTimer = freeRunningTimer;
                toaster.state = COOKING;
                LEDS_SET(LEDActivation[8]);
                updateOvenOLED(toaster);
                
            }
            break;
        case SELECTOR_CHANGE_PENDING:
            if (Timer1.buttonEvent & BUTTON_EVENT_3UP) {
                if (freeRunningTimer - startTimer >= SECOND) {
                    toaster.selector = (toaster.selector == TIME) ? TEMPERATURE : TIME;
                }
                else {
                    if (toaster.mode == BAKE) toaster.mode = TOAST;
                    else if (toaster.mode = TOAST) {
                        toaster.mode = BROIL;
                        toaster.temperature = BROIL_TEMP;
                    }
                    else {
                        toaster.mode = BAKE;
                        toaster.temperature = BAKE_TEMP;
                    }
                }
                updateOvenOLED(toaster);
                toaster.state = SETUP;   
            } 
            break;
        case COOKING:
            if (Timer1.buttonEvent & BUTTON_EVENT_4DOWN) {
                if (TIMER_TICK) {
                    setLEDs();
                    toaster.remainingTime--;
                    updateOvenOLED(toaster);                   
                }
                startTimer = freeRunningTimer;
                toaster.state = RESET_PENDING;
            }
                
            break;
        case RESET_PENDING:
            if (Timer1.buttonEvent & BUTTON_EVENT_4DOWN) {
                if (TIMER_TICK) {
                    if (freeRunningTimer - startTimer >= SECOND) {
                        toaster.state = SETUP;
                        toaster.remainingTime = toaster.initTime;
                        LEDS_SET(LEDActivation[0]);
                    }
                    else {
                        setLEDs();
                    }
                    toaster.remainingTime--;
                    updateOvenOLED(toaster);
                }
            }
            else if (Timer1.buttonEvent & BUTTON_EVENT_4UP) {
                toaster.state = COOKING;
            }

            break;
    }
    //write your SM logic here.
}


int main()
{
    BOARD_Init();

     //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">
    
    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
   
    printf("Welcome to CRUZID's Lab05 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here
    Timer1.buttonEvent = BUTTON_EVENT_NONE;
    Timer1.adcEvent = FALSE;
    TIMER_TICK = FALSE;
    toaster.state = SETUP;
    toaster.mode = BAKE;
    toaster.temperature = BAKE_TEMP;
    
    AdcInit();
    ButtonsInit();
    OledInit();
    LEDS_INIT();
    
    LEDS_SET(LEDActivation[0]);
    while (1){
       
        // Add main loop code here:
        // check for events
        // on event, run runOvenSM()
        // clear event flags
        if (Timer1.buttonEvent != BUTTON_EVENT_NONE || Timer1.adcEvent == TRUE || TIMER_TICK) {
            runOvenSM();
            TIMER_TICK = FALSE;
            Timer1.buttonEvent = BUTTON_EVENT_NONE;
            Timer1.adcEvent = FALSE;
        }
            
    };
}


/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    
    TIMER_TICK = TRUE;
    freeRunningTimer++;

    //add event-checking code here
}


/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    Timer1.buttonEvent = ButtonsCheckEvents();
    Timer1.adcEvent = AdcChanged();
    //add event-checking code here
}