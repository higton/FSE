#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "mqtt.h"
#include "cJSON.h"
#include "three_clor.h"

// typedef struct led_info {
//     bool red;
//     bool green;
//     bool blue;
// }led_info;

// led_info led_status

// #define LED GPIO_NUM_2
#define LED_RED GPIO_NUM_4
#define LED_GREEN GPIO_NUM_5
#define LED_BLUE GPIO_NUM_18

void config_led()
{
  gpio_pad_select_gpio(LED_RED);
  gpio_pad_select_gpio(LED_GREEN);
  gpio_pad_select_gpio(LED_BLUE);
  gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED_BLUE, GPIO_MODE_OUTPUT);
}

void setup_red()
{
  gpio_set_level(LED_BLUE, LOW);
  gpio_set_level(LED_GREEN, LOW);
  gpio_set_level(LED_RED, HIGH);
}

void setup_green()
{
  gpio_set_level(LED_BLUE, LOW);
  gpio_set_level(LED_GREEN, HIGH);
  gpio_set_level(LED_RED, LOW);
}

void setup_blue()
{
  gpio_set_level(LED_BLUE, HIGH);
  gpio_set_level(LED_GREEN, LOW);
  gpio_set_level(LED_RED, LOW);
}