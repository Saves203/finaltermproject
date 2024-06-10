#include "light.h"

static const char *TAG = "example";

void configure_led(gpio_num_t pin) {
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}