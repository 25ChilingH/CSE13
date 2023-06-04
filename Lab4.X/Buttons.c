#include "Buttons.h"
#include "BOARD.h"

uint8_t counter;
uint8_t prevButtonsState;

void ButtonsInit(void) {
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
    
    counter = 0;
    prevButtonsState = 0x00;
}

uint8_t ButtonsCheckEvents(void) {
    uint8_t buttonsState = BUTTON_STATES();
    
    if (counter >= BUTTONS_DEBOUNCE_PERIOD) {
        uint8_t ans = BUTTON_EVENT_NONE;
        
        if ((prevButtonsState & BUTTON_STATE_4) && !(buttonsState & BUTTON_STATE_4)) {
            ans |= BUTTON_EVENT_4UP;
        }
        else if (!(prevButtonsState & BUTTON_STATE_4) && (buttonsState & BUTTON_STATE_4)) {
            ans |= BUTTON_EVENT_4DOWN;
        }
        
        if ((prevButtonsState & BUTTON_STATE_3) && !(buttonsState & BUTTON_STATE_3)) {
            ans |= BUTTON_EVENT_3UP;
        }
        else if (!(prevButtonsState & BUTTON_STATE_3) && (buttonsState & BUTTON_STATE_3)) {
            ans |= BUTTON_EVENT_3DOWN;
        }
        
        
        if ((prevButtonsState & BUTTON_STATE_2) && !(buttonsState & BUTTON_STATE_2)) {
            ans |= BUTTON_EVENT_2UP;
        }
        else if (!(prevButtonsState & BUTTON_STATE_2) && (buttonsState & BUTTON_STATE_2)) {
            ans |= BUTTON_EVENT_2DOWN;
        }
        
        
        if ((prevButtonsState & BUTTON_STATE_1) && !(buttonsState & BUTTON_STATE_1)) {
            ans |= BUTTON_EVENT_1UP;
        }
        else if (!(prevButtonsState & BUTTON_STATE_1) && (buttonsState & BUTTON_STATE_1)) {
            ans |= BUTTON_EVENT_1DOWN;
        }
        
        return ans;
        
    }
    counter++;
    counter %= BUTTONS_DEBOUNCE_PERIOD;
    prevButtonsState = buttonsState;
    
    return BUTTON_EVENT_NONE;

}