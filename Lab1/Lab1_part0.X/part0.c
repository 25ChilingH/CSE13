/* 
 * File:   part0.c
 * Author: chilinghan
 *
 * Created on April 9, 2023, 6:44 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"

/*
 * 
 */
int main(int argc, char** argv) {
    BOARD_Init();
    printf("Hello World\n");
    
    BOARD_End();
    while(1);
}

