// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"


// User libraries
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    BOARD_Init();
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    float kelvin, fahrenheit;
    int lower, upper, step;

    lower = 0;
    upper = 300;
    step = 20;
    kelvin = lower;
    printf("%5c %5c\n", 'K', 'F');
    while (kelvin <= upper) {
        fahrenheit = (kelvin - 273.15) * 9 / 5 + 32;
        printf("%7.1f %04.0f\n", (double) kelvin, (double) fahrenheit);
        kelvin += step;
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     **************************************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}
