#include "clock.h"

#include "inc/msp432p401r.h"

// Configure the system clock to run at the fastest
// and most accurate settings.
uint32_t pre_wait = 0;        // loops between BSP_Clock_InitFastest() called and PCM idle (expect 0)
uint32_t cpm_wait = 0;        // loops between Power Active Mode Request and Current Power Mode matching requested mode (expect small)
uint32_t post_wait = 0;       // loops between Current Power Mode matching requested mode and PCM module idle (expect about 0)
uint32_t iflags = 0;         // non-zero if transition is invalid
uint32_t crystal_stable = 0;  // loops before the crystal stabilizes (expect small)

void ClockInit48MHz(void) {
    // wait for the PCMCTL0 and Clock System to be write-able by waiting for Power Control Manager to be idle
    while (PCM->CTL1 & 0x00000100) {
        pre_wait++;
        if (pre_wait >= 100000) {
            return;  // time out error
        }
    }
    // request power active mode LDO VCORE1 to support the 48 MHz frequency
    PCM->CTL0 = (PCM->CTL0 & ~0xFFFF000F) |  // clear PCMKEY bit field and AMR bit field
                0x695A0000 |                 // write the proper PCM key to unlock write access
                0x00000001;                  // request power active mode LDO VCORE1
    // check if the transition is invalid (see Figure 7-3 on p344 of datasheet)
    if (PCM->IFG & 0x00000004) {
        iflags = PCM->IFG;         // bit 2 set on active mode transition invalid; bits 1-0 are for LPM-related errors; bit 6 is for DC-DC-related error
        PCM->CLRIFG = 0x00000004;  // clear the transition invalid flag
        // to do: look at CPM bit field in PCMCTL0, figure out what mode you're in, and step through the chart to transition to the mode you want
        // or be lazy and do nothing; this should work out of reset at least, but it WILL NOT work if Clock_Int32kHz() or Clock_InitLowPower() has been called
        return;
    }
    // wait for the CPM (Current Power Mode) bit field to reflect a change to active mode LDO VCORE1
    while ((PCM->CTL0 & 0x00003F00) != 0x00000100) {
        cpm_wait++;
        if (cpm_wait >= 500000) {
            return;  // time out error
        }
    }
    // wait for the PCMCTL0 and Clock System to be write-able by waiting for Power Control Manager to be idle
    while (PCM->CTL1 & 0x00000100) {
        post_wait++;
        if (post_wait >= 100000) {
            return;  // time out error
        }
    }
    // initialize PJ.3 and PJ.2 and make them HFXT (PJ.3 built-in 48 MHz crystal out; PJ.2 built-in 48 MHz crystal in)
    PJ->SEL0 |= 0x0C;
    PJ->SEL1 &= ~0x0C;                     // configure built-in 48 MHz crystal for HFXT operation
    CS->KEY = 0x695A;                      // unlock CS module for register access
    CS->CTL2 = (CS->CTL2 & ~0x00700000) |  // clear HFXTFREQ bit field
               0x00600000 |                // configure for 48 MHz external crystal
               0x00010000 |                // HFXT oscillator drive selection for crystals >4 MHz
               0x01000000;                 // enable HFXT
    CS->CTL2 &= ~0x02000000;               // disable high-frequency crystal bypass
    // wait for the HFXT clock to stabilize
    while (CS->IFG & 0x00000002) {
        CS->CLRIFG = 0x00000002;  // clear the HFXT oscillator interrupt flag
        crystal_stable++;
        if (crystal_stable > 100000) {
            return;  // time out error
        }
    }
    // configure for 2 wait states (minimum for 48 MHz operation) for flash Bank 0
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~0x0000F000) | FLCTL_BANK0_RDCTL_WAIT_2;
    // configure for 2 wait states (minimum for 48 MHz operation) for flash Bank 1
    FLCTL->BANK1_RDCTL = (FLCTL->BANK1_RDCTL & ~0x0000F000) | FLCTL_BANK1_RDCTL_WAIT_2;
    CS->CTL1 = 0x20000000 |  // configure for SMCLK divider /4
               0x00100000 |  // configure for HSMCLK divider /2
               0x00000200 |  // configure for ACLK sourced from REFOCLK
               0x00000050 |  // configure for SMCLK and HSMCLK sourced from HFXTCLK
               0x00000005;   // configure for MCLK sourced from HFXTCLK
    CS->KEY = 0;             // lock CS module from unintended access
}
