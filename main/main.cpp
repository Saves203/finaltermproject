
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"


#include "jsoncpp/value.h"
#include "jsoncpp/json.h"

#include "esp_firebase/app.h"
#include "esp_firebase/rtdb.h"

#include "wifi_utils.h"

#include "firebase_config.h"

#include "light.h"
#include "motor.h"
#include "dht11.h"

using namespace ESPFirebase;
extern "C" void app_main(void)
{
    wifiInit(SSID, PASSWORD);  // blocking until it connects

    // Config and Authentication
    user_account_t account = {USER_EMAIL, USER_PASSWORD};

    FirebaseApp app = FirebaseApp(API_KEY);

    app.loginUserAccount(account);

    RTDB db = RTDB(&app, DATABASE_URL);

    
    configure_led(GPIO_NUM_2); //led_pin
    configure_led(GPIO_NUM_4); //lock_pin
    configure_led(GPIO_NUM_18);//fan_pin
    motor_init();
    DHT11_init(GPIO_NUM_5);

    while(1){
        //---------------led_control-----------------
        Json::Value led_state = db.getData("light");
        if(led_state == "on")
            gpio_set_level(GPIO_NUM_2, 1);
        else
            gpio_set_level(GPIO_NUM_2, 0);
        //---------------lock_control----------------
        Json::Value lock_state = db.getData("lock");
        if(lock_state == "open")
            gpio_set_level(GPIO_NUM_4, 1);
        else
            gpio_set_level(GPIO_NUM_4, 0);
        //---------------fan_control-----------------
        
        /*
        Json::Value fan_state = db.getData("fan");
        if(fan_state == "on")
            gpio_set_level(GPIO_NUM_18, 1);
        else
            gpio_set_level(GPIO_NUM_18, 0);
        */
        Json::Value fan = db.getData("motor");
        Json::FastWriter writer;
        std::string fan_string = writer.write(fan);


        Json::Value data;
        Json::Reader reader;
        reader.parse(fan_string, data);
        std::uint32_t speed = data["motorspeed"].asUInt();
        std::string fanstate = data["motostate"].asString();
 
        if (fanstate == "on"){
            motor_set_speed(speed);
            motor_forward();
        }else{
            gpio_set_level(GPIO_NUM_22, 0);
            gpio_set_level(GPIO_NUM_23, 0);
        }
        //---------------dht11_read------------------
        db.putData("/humidity", DHT11_read().humidity);
        db.putData("/temperature", DHT11_read().temperature);
        vTaskDelay(750 / portTICK_PERIOD_MS);
    }

}

