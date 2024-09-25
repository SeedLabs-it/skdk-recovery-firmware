#pragma once

#if SK_WIFI
#include "task.h"
#include <logging.h>
// #include "events/events.h"
#include <ElegantOTAPro.h>

class WiFiTask : public Task<WiFiTask> {
    friend class Task<WiFiTask>; // Allow base Task to invoke protected run()

  public:
    WiFiTask(const uint8_t task_core);
    ~WiFiTask();

  protected:
    void run();

  private:
    WiFiClient wifi_client;
    WebServer *server_;

    // QueueHandle_t wifi_event_queue = xQueueCreate(2, sizeof(Event));

    void eventHandler(Event event) override;

    void handleRoot();
    void publishEvent(Event event);
    static void WiFiEventHandler(WiFiEvent_t event);
};
#else

class WiFiTask {};

#endif
