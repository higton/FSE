#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJson.h"
#include "mqtt.h"

#include "dht11.h"
#include "dht_handle.h"
#include "nvs_handle.h"

#define TAG "DHT11"
#define DHT11_PIN GPIO_NUM_4

#define AVERAGE 0
#define LATEST 1

int dht11_enabled = 1;

void mqtt_publish_dht(void *temperature, void *humidity, int type)
{
    // Create a JSON object
    cJSON *data = cJSON_CreateObject();
    if (data == NULL)
    {
        ESP_LOGE(TAG, "Error creating JSON");
        return;
    }

    // Add temperature and humidity values to the JSON object
    if (type == AVERAGE)
    {
        float avg_temp = *(float *)temperature;
        float avg_hum = *(float *)humidity;

        cJSON_AddItemToObject(data, "average_temperature", cJSON_CreateNumber(avg_temp));
        cJSON_AddItemToObject(data, "average_humidity", cJSON_CreateNumber(avg_hum));
        mqtt_publish("v1/devices/me/telemetry", cJSON_Print(data));
    }
    else
    {
        int latest_temp = *(int *)temperature;
        int latest_hum = *(int *)humidity;
        cJSON_AddItemToObject(data, "temperature", cJSON_CreateNumber(latest_temp));
        cJSON_AddItemToObject(data, "humidity", cJSON_CreateNumber(latest_hum));
        mqtt_publish("v1/devices/me/telemetry", cJSON_Print(data));
    }
}

void read_dht() {
    float avg_temperature = 0.0;
    float avg_humidity = 0.0;
    int count = 0;
    struct dht11_reading dht11_data;

    while (true) {
        if (dht11_enabled) {
            dht11_data = DHT11_read();
            if (dht11_data.status == DHT11_OK) {
                avg_temperature += dht11_data.temperature;
                avg_humidity += dht11_data.humidity;
                ESP_LOGI(TAG, "Latest Reading: [Temperature]: %d | [Humidity]: %d",
                         dht11_data.temperature, dht11_data.humidity);
                mqtt_publish_dht(&dht11_data.temperature, &dht11_data.humidity, LATEST);
                count++;
                if (count == 10) {
                    avg_temperature /= count;
                    avg_humidity /= count;
                    ESP_LOGI(TAG, "Average Reading: [Temperature]: %.2f | [Humidity]: %.2f",
                             avg_temperature, avg_humidity);
                    mqtt_publish_dht(&avg_temperature, &avg_humidity, AVERAGE);
                    count = 1;
                }
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


void setup_dht()
{
    DHT11_init(DHT11_PIN);
    xTaskCreate(&read_dht, "Read DHT11 Task", 4096, NULL, 1, NULL);
}
