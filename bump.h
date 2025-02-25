#ifndef BUMP_H
#define BUMP_H

#include <stdint.h>

// Set port pins Port 4.0, 4.2, 4.3, 4.5, 4.6 and 4.7 and enable internal resistors as needed
void BumpInit(void);

// Read current state of 6 switches
// Returns a 6-bit positive logic result (0 to 63)
// bit 5 Bump5
// bit 4 Bump4
// bit 3 Bump3
// bit 2 Bump2
// bit 1 Bump1
// bit 0 Bump0 
uint8_t BumpRead(void);

#endif /* BUMP_H */