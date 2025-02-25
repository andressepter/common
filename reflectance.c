#include "reflectance.h"

#include "delay.h"

#include "inc/msp432p401r.h"

void ReflectanceInit(void) {
    // write this as part of Lab 3
}

uint8_t ReflectanceRead(uint32_t time) {
    // write this as part of Lab 3
    return 0;
}

int32_t ReflectancePosition(uint8_t sensor_data) {
    // write this as part of Lab 3
    return 0;
}

void ReflectanceStart(void) {
    // write this as part of Lab 3
    // turn on 4 even IR LEDs
    // turn on 4 odd IR LEDs
    // make P7.7-P7.0 out
    // prime for measurement
    // wait 10 us
    // make P7.7-P7.0 in
}

uint8_t ReflectanceEnd(void) {
    // write this as part of Lab 3
    // convert P7.0 input to digital
    // turn off 4 even IR LEDs
    // turn off 4 odd IR LEDs
    return 0; // replace this line
}
