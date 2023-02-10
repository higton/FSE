#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "mqtt.h"
#include "cJson.h"
#include "pwm.h"

#define TAG "MQTT"
#define MQTT_USERNAME CONFIG_ESP_MQTT_ACESS_TOKEN
#define MQTT_URI CONFIG_ESP_MQTT_URI

extern xSemaphoreHandle mqttSemaphore;
esp_mqtt_client_handle_t client;

int mqtt_subscribe(char *topic);
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_event_data_handler(char *data);

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        xSemaphoreGive(mqttSemaphore);
        esp_err_t err = mqtt_subscribe("v1/devices/me/rpc/request/+");
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Error subscribing to topic: %d", err);
            break;
        } else {
            ESP_LOGI(TAG, "Subscribed to topic");
        }
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        mqtt_event_data_handler(event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_event_data_handler(char *data) {
    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
        return;
    char *key = cJSON_GetObjectItem(json, "method")->valuestring;
    int value = cJSON_GetObjectItem(json, "params")->valueint;
    if(strstr(key, "setPWM") != NULL){
        // log the value
        ESP_LOGI(TAG, "Received value pwm: %d", value);
        set_pwm(value);
    }
}

void mqtt_start() {
    esp_mqtt_client_config_t mqtt_config = {
        .uri = MQTT_URI,
        .username = MQTT_USERNAME
    };
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mqtt_publish(char *topic, char *message) {
    int message_id = esp_mqtt_client_publish(client, topic, message, 0, 1, 0);
    ESP_LOGI(TAG, "Published to topic: %s, ID: %d", topic, message_id);
}

int mqtt_subscribe(char *topic) {
    int message_id = esp_mqtt_client_subscribe(client, topic, 0);
    return message_id;
}

void mqtt_restart() {
    esp_mqtt_client_start(client);
    ESP_LOGI(TAG, "Restart");
}

void mqtt_stop() {
    esp_mqtt_client_stop(client);
    ESP_LOGI(TAG, "Stop");
}