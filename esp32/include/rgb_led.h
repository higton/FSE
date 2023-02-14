#ifndef __RGB_LED_H__
#define __RGB_LED_H__
#include <stdint.h>

#define HIGH 1
#define LOW 0

void config_led();

void setup_red();
void setup_green();
void setup_blue();

#endif