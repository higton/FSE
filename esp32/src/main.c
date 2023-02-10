#include <stdio.h>
#include "esp_event.h"
#include "freertos/semphr.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

#include "pwm.h"
#include "connections.h"
#include "nvs_flash.h"


xSemaphoreHandle wifiSemaphore;
xSemaphoreHandle wifiReconnectSemaphore;
xSemaphoreHandle mqttSemaphore;

void app_main()
{
  mqttSemaphore = xSemaphoreCreateBinary();
  wifiSemaphore = xSemaphoreCreateBinary();
  wifiReconnectSemaphore = xSemaphoreCreateBinary();

  nvs_flash_init();
  connections();
}