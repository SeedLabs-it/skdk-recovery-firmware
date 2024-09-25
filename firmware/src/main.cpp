#include "root_task.h"
#include <Arduino.h>

// #if SK_MQTT
// static MqttTask mqtt_task(1);
// static MqttTask *mqtt_task_p = &mqtt_task;
// #else
// static MqttTask *mqtt_task_p = nullptr;

// #endif

// RootTask root_task(0, &config, motor_task, display_task_p, wifi_task_p,
// mqtt_task_p, led_ring_task_p, sensors_task_p, reset_task_p);
RootTask root_task(0);

void setup() {
    root_task.begin();

    // Free up the Arduino loop task
    vTaskDelete(NULL);
}

void loop() {}