#include "root_task.h"

#if SK_DISPLAY
static DisplayTask display_task(0);
// static DisplayTask *display_task_p = &display_task;
#else
static DisplayTask *display_task_p = nullptr;
#endif

#if SK_WIFI
static WiFiTask wifi_task(1);
// static WiFiTask *wifi_task_p = &wifi_task;
#else
static WiFiTask *wifi_task_p = nullptr;
#endif

RootTask::RootTask(const BaseType_t coreId) : Task("RootTask", 1024 * 8, 1, coreId) {}
RootTask::~RootTask() {}

void RootTask::run() {
    stream_.begin(115200);
    stream_.setTxBufferSize(1024);
    delay(500); // Allow time for serial to initialize
#if !SK_LOGGING
    Logging::setAdapter(new FreeRTOSAdapter(new SerialProtocolPlaintext(stream_), "FreeRTOSAdapter", 1024 * 16, 0, 0));
#endif
    delay(250); // Allow time for logging to initialize

    LOG("RootTask::run");

    // TODO move function etc
    //  LED STUFF
    CRGB leds[NUM_LEDS];
    FastLED.addLeds<WS2812B, PIN_LED_DATA, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(5);
    FastLED.clear();
    for (size_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::OrangeRed;
    }
    FastLED.show();
    // END LED STUFF

    wifi_task.begin();
    WiFi.onEvent(WiFiEventHandler);
    display_task.begin();

    unsigned long last_dim_brighten = millis();
    bool up = true;

    while (1) {
        if (xQueueReceive(event_queue, &event, 0)) {
            eventHandler(event);
        }

        // TODO MOVE SOMEWHERE ELSE
        if (millis() - last_dim_brighten > 20) {
            last_dim_brighten = millis();
            uint8_t brightness = FastLED.getBrightness();

            if (!up && brightness <= 5) {
                up = true;
            } else if (up && brightness >= 200) {
                up = false;
            }

            if (up) {
                FastLED.setBrightness(brightness + 1);
            } else {
                FastLED.setBrightness(brightness - 1);
            }
            FastLED.show();
            LOGE("Brightness: %d", FastLED.getBrightness());
        }
        delay(5);
    }
}

void RootTask::eventHandler(Event event) {
    switch (event.type) {
    case SK_WIFI_AP_STARTED:
        LOGV(LOG_LEVEL_DEBUG, "SK_WIFI_AP_STARTED");
        break;
    case SK_WIFI_CLIENT_CONNECTED:
        LOGV(LOG_LEVEL_DEBUG, "SK_WIFI_CLIENT_CONNECTED");
        display_task.enqueueEvent(event);
        break;
    case SK_WIFI_CLIENT_DISCONNECTED:
        LOGV(LOG_LEVEL_DEBUG, "SK_WIFI_CLIENT_DISCONNECTED");
        display_task.enqueueEvent(event);
        break;
    default:
        LOGV(LOG_LEVEL_WARNING, "Unhandeled event, id: %d", event.type);
        break;
    }
}

void RootTask::WiFiEventHandler(WiFiEvent_t event) {

    Event event_;
    switch (event) {
    case ARDUINO_EVENT_WIFI_AP_START:
        event_.type = SK_WIFI_AP_STARTED;
        xQueueSend(event_queue, &event_, portMAX_DELAY);
        break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
        event_.type = SK_WIFI_CLIENT_CONNECTED;
        xQueueSend(event_queue, &event_, portMAX_DELAY);
        break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
        event_.type = SK_WIFI_CLIENT_DISCONNECTED;
        xQueueSend(event_queue, &event_, portMAX_DELAY);
        break;
    default:
        break;
    }
}