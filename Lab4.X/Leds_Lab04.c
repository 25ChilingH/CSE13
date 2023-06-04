#include <proc/p32mx340f512h.h>

#include "Leds_Lab04.h"

void LEDS_INIT(void) {
    TRISE = 0x00;
    LATE = 0x00;
}


void LEDS_SET(char newPattern) {
    LATE = newPattern;
    
}

char LEDS_GET(void) {
    return LATE;
}

