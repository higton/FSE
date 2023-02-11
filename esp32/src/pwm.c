#include "freertos/FreeRTOS.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "pwm.h"
#include "nvs_handle.h"

#define LED GPIO_NUM_2

#define TAG "PWM"

void set_pwm_from_nvs() {
    int value = read_nvs_value("pwm");
    if (value != -1)
    {
        set_pwm(value);
    }
}

void write_pwm_to_nvs(int32_t value) {
    write_nvs_value("pwm", value);
}

void config_pwm() {
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK

    };
    ledc_timer_config(&timer_config);
    ledc_channel_config_t channel_config = {
        .gpio_num = LED,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0};
    ledc_channel_config(&channel_config);
    set_pwm_from_nvs();
}

int get_pwm_value() {
    return ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void set_pwm(int value) {
    // log info
    ESP_LOGI(TAG, "Setting PWM to %d", value);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, value*2.5);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    write_pwm_to_nvs(value);
}