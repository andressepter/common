#include "bump.h"

#include "inc/msp432p401r.h"

void BumpInit(void) {
        // Set Port 4 pins 0, 2, 3, 5, 6, 7 as inputs
        P4->DIR &= ~(BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7);
        
        // Enable pull-up resistors for these pins
        P4->REN |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7);
        P4->OUT |= (BIT0 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7);
}

uint8_t BumpRead(void) {
    uint8_t result = 0;
    
    // Read the state of the bump sensors
    result |= (P4->IN & BIT0) ? 0x01 : 0x00;
    result |= (P4->IN & BIT2) ? 0x02 : 0x00;
    result |= (P4->IN & BIT3) ? 0x04 : 0x00;
    result |= (P4->IN & BIT5) ? 0x08 : 0x00;
    result |= (P4->IN & BIT6) ? 0x10 : 0x00;
    result |= (P4->IN & BIT7) ? 0x20 : 0x00;
    
    return result;
}
