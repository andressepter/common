#include "delay.h"

void __attribute__((naked))
Delay(uint32_t count) {
    __asm("    subs    r0, #1\n"
          "    bne     Delay\n"
          "    bx      lr");
}

// 1us tuned at 48 MHz
void Delay1us(uint32_t n) {
    for (n = (382 * n) / 100; n; n--) {}
}

// 1 msec, tuned at 48 MHz
void Delay1ms(uint32_t n) {
    for (; n; n--) {
        Delay(48000000 / 9162);
    }
}