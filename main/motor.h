#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void motor_init();
void motor_set_speed(uint32_t speed);
void motor_forward();
void motor_backward();
void motor_stop();
void motor_deinit();
#endif