#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "nvs_handle.h"

#define NVS_NAMESPACE "storage"
#define LOG_TAG "NVS"

int32_t read_nvs_value(char *attribute) {
  nvs_handle handle;
  esp_err_t nvs_result = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
  int32_t value = 0;

  if (nvs_result == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGE(LOG_TAG, "Namespace: %s not found", NVS_NAMESPACE);
  } else {
    esp_err_t read_result = nvs_get_i32(handle, attribute, &value);

    switch (read_result) {
      case ESP_OK:
        ESP_LOGI(LOG_TAG, "Value obtained");
        break;
      case ESP_ERR_NOT_FOUND:
        ESP_LOGE(LOG_TAG, "Value not found\n");
        return -1;
      default:
        ESP_LOGE(LOG_TAG, "Error accessing NVS (%s)", esp_err_to_name(read_result));
        return -1;
    }

    nvs_close(handle);
  }

  return value;
}

void write_value_to_nvs(char *attribute, int32_t value) {
  nvs_handle handle;
  esp_err_t nvs_result = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);

  if (nvs_result == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGE(LOG_TAG, "Namespace: %s not found", NVS_NAMESPACE);
  }

  esp_err_t write_result = nvs_set_i32(handle, attribute, value);

  if (write_result != ESP_OK) {
    ESP_LOGE(LOG_TAG, "Unable to write attribute %s to NVS (%s)", attribute, esp_err_to_name(write_result));
  }

  nvs_commit(handle);
  nvs_close(handle);
}
