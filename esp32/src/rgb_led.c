#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "mqtt.h"
#include "cJSON.h"
#include "nvs_handle.h"
#include "rgb_led.h"

// #define LED GPIO_NUM_2
#define LED_RED GPIO_NUM_4
#define LED_GREEN GPIO_NUM_5
#define LED_BLUE GPIO_NUM_18

void set_led_from_nvs() {
    int value = read_nvs_value("led");
    if (value != -1)
    {
      if(value == 1)
        setup_red();
      if(value == 2)
        setup_green();
      if(value == 3)
        setup_blue();
    }
}

void write_led_to_nvs(int32_t value) {
    write_value_to_nvs("led", value);
}

void config_led()
{
  gpio_pad_select_gpio(LED_RED);
  gpio_pad_select_gpio(LED_GREEN);
  gpio_pad_select_gpio(LED_BLUE);
  gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED_BLUE, GPIO_MODE_OUTPUT);

  set_led_from_nvs();
}

void setup_red()
{
  gpio_set_level(LED_BLUE, LOW);
  gpio_set_level(LED_GREEN, LOW);
  gpio_set_level(LED_RED, HIGH);
  write_led_to_nvs(1);
}

void setup_green()
{
  gpio_set_level(LED_BLUE, LOW);
  gpio_set_level(LED_GREEN, HIGH);
  gpio_set_level(LED_RED, LOW);
  write_led_to_nvs(2);
}

void setup_blue()
{
  gpio_set_level(LED_BLUE, HIGH);
  gpio_set_level(LED_GREEN, LOW);
  gpio_set_level(LED_RED, LOW);
  write_led_to_nvs(3);
}