#include "motor.h"

#define MOTOR_ENA_PIN GPIO_NUM_21
#define MOTOR_IN1_PIN GPIO_NUM_22
#define MOTOR_IN2_PIN GPIO_NUM_23 
#define MOTOR_PWM_CHANNEL LEDC_CHANNEL_1
#define MOTOR_PWM_FREQ 1000
#define MOTOR_PWM_RESO LEDC_TIMER_10_BIT

void motor_init() {
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

    io_conf.pin_bit_mask = (1ULL << MOTOR_ENA_PIN);
    gpio_config(&io_conf);

    io_conf.pin_bit_mask = (1ULL << MOTOR_IN1_PIN) | (1ULL << MOTOR_IN2_PIN);
    gpio_config(&io_conf);

    ledc_timer_config_t ledc_timer;
    ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_0;
    ledc_timer.duty_resolution = MOTOR_PWM_RESO;
    ledc_timer.freq_hz = MOTOR_PWM_FREQ;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel;
    ledc_channel.channel = MOTOR_PWM_CHANNEL;
    ledc_channel.duty = 0;
    ledc_channel.gpio_num = MOTOR_ENA_PIN;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.hpoint = 0;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_config(&ledc_channel);
}

void motor_set_speed(uint32_t speed){
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, MOTOR_PWM_CHANNEL, speed);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, MOTOR_PWM_CHANNEL);
}

void motor_forward(){
    gpio_set_level(MOTOR_IN1_PIN, 1);
    gpio_set_level(MOTOR_IN2_PIN, 0);
}

void motor_backward(){
    gpio_set_level(MOTOR_IN1_PIN, 0);
    gpio_set_level(MOTOR_IN2_PIN, 1);
}

void motor_stop(){
    gpio_set_level(MOTOR_IN1_PIN, 0);
    gpio_set_level(MOTOR_IN2_PIN, 0);
}

void motor_deinit(){
    ledc_stop(LEDC_HIGH_SPEED_MODE, MOTOR_PWM_CHANNEL, 0);

    gpio_set_level(MOTOR_IN1_PIN, 0);
    gpio_set_level(MOTOR_IN2_PIN, 0);

    gpio_reset_pin(MOTOR_ENA_PIN);
    gpio_reset_pin(MOTOR_IN1_PIN);
    gpio_reset_pin(MOTOR_IN2_PIN);
}

/*
    void app_main(){
        motor_init();
        while(1){
            motor_set_speed(500);
            motor_forward();
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            motor_set_speed(500);
            motor_backward();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
*/