#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp32/rom/uart.h"
#include "driver/gpio.h"
#include "cJson.h"

#include "driver/rtc_io.h"
#include "wifi.h"
#include "mqtt.h"
#include "pwm.h"
#include "dht_handle.h"

#define TAG "LOW POWER"
#define BUTTON 0

xSemaphoreHandle wifiReconnectSemaphore;

cJSON * create_attributes_json()
{
    cJSON *data = cJSON_CreateObject();
    if (data == NULL)
    {
        ESP_LOGE("LOW POWER", "Erro ao criar JSON");
        return NULL;
    }
    cJSON_AddItemToObject(data, "temperature", cJSON_CreateNumber(batterie_get_dht11_data("temperature")));
    cJSON_AddItemToObject(data, "humidity", cJSON_CreateNumber(batterie_get_dht11_data("humidity")));
    cJSON_AddItemToObject(data, "status_led", cJSON_CreateNumber(get_pwm_value()));

    return data;

}
void batterie_mode()
{
    // Configuração da GPIO para o botão de entrada
    gpio_pad_select_gpio(BUTTON);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    // Habilita o botão para acordar a placa
    gpio_wakeup_enable(BUTTON, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    cJSON *data = cJSON_CreateObject();

    ESP_LOGI("SYS", "SLEEPING");
    while(true)
    {
        mqtt_stop();
        wifi_stop();

        printf("Entrando em modo Light Sleep\n");
        
        // Configura o modo sleep somente após completar a escrita na UART para finalizar o printf
        uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

        // Entra em modo Light Sleep
        esp_light_sleep_start();

        printf("Acordou do modo Light Sleep\n");

        if (rtc_gpio_get_level(BUTTON) == 0)
        {
            printf("Aguardando soltar o botão ... \n");
            do
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            } while (rtc_gpio_get_level(BUTTON) == 0);

            wifi_restart();
            if (xSemaphoreTake(wifiReconnectSemaphore, portMAX_DELAY))
            {
                mqtt_restart();
                data = create_attributes_json();
                if(data != NULL)
                    mqtt_publish("v1/devices/me/attributes", cJSON_Print(data));
            }

            vTaskDelay(pdMS_TO_TICKS(100));
        }

    }

}

