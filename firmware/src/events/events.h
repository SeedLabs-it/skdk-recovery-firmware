#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdint.h>

struct WiFiConfiguration {
    char ssid[128];
    char passphrase[128];
};

union EventBody {
    WiFiConfiguration wifi_configuration;
};

// TODO, think events more careful, for example add SK_MQTT_CREDENTIALS_RECIEVED
// TODO add uniq prefix, clashing with some events
enum EventType {
    SK_WIFI_AP_STARTED = 1,
    SK_WIFI_CLIENT_CONNECTED,
    SK_WIFI_CLIENT_DISCONNECTED,
};

typedef unsigned long SentAt;

struct Event {
    EventType type;
    EventBody body;
    SentAt sent_at;
};

extern QueueHandle_t event_queue;