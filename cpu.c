#include "cpu.h"

uint32_t __attribute__((naked)) CPU_cpsie(void)
{
    uint32_t ret;

    //
    // Read PRIMASK and enable interrupts.
    //
    __asm("    mrs     r0, PRIMASK\n"
            "    cpsie   i\n"
            "    bx      lr\n"
            : "=r" (ret));

    //
    // The return is handled in the inline assembly, but the compiler will
    // still complain if there is not an explicit return here (despite the fact
    // that this does not result in any code being produced because of the
    // naked attribute).
    //
    return(ret);
}