#include <stdio.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "connections.h"
#include "wifi.h"
#include "mqtt.h"
#include "pwm.h"
#include "rgb_led.h"
#include "dht_handle.h"
#include "batterie_mode.h"

#define WIFI_CONNECT_DELAY_MS 5000
#define WIFI_CONNECT_ATTEMPTS 3
#define ESP_MODE CONFIG_ESP_POWER_MODE
#define BATTERY_MODE 0
#define AC_MODE 1

extern xSemaphoreHandle wifiSemaphore;
extern xSemaphoreHandle mqttSemaphore;

static void wifiConnectTask(void* arg)
{
    int connectAttempts = 0;
    while (connectAttempts < WIFI_CONNECT_ATTEMPTS) {
        if (xSemaphoreTake(wifiSemaphore, portMAX_DELAY)) {
            ESP_LOGI("WifiConnectTask", "Wifi connection established, starting MQTT");
            mqtt_start();
            break;
        }
        ESP_LOGE("WifiConnectTask", "Failed to connect to wifi, retrying in %dms", WIFI_CONNECT_DELAY_MS);
        vTaskDelay(WIFI_CONNECT_DELAY_MS / portTICK_PERIOD_MS);
        connectAttempts++;
    }
    if (connectAttempts == WIFI_CONNECT_ATTEMPTS) {
        ESP_LOGE("WifiConnectTask", "Failed to connect to wifi after %d attempts", WIFI_CONNECT_ATTEMPTS);
    }
    vTaskDelete(NULL);
}

void connections()
{

  wifi_start();
  xTaskCreate(&wifiConnectTask, "WifiConnectTask", 4096, NULL, 1, NULL);
  if (xSemaphoreTake(mqttSemaphore, portMAX_DELAY))
  {
    config_pwm();
    config_led();
    if (ESP_MODE == BATTERY_MODE)
    {
      ESP_LOGI("Modo Energia", "Funcionamento via bateria");
      xTaskCreate(batterie_mode, "light_sleep_task", 4096, NULL, 1, NULL);
    } else if (ESP_MODE == AC_MODE)
    {
      setup_dht();
    }
  }
}
