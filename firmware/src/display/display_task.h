#pragma once

#if SK_DISPLAY
#include "./display/driver/lv_skdk.h"
#include "logging.h"
#include "task.h"
#include <WiFi.h>

class DisplayTask : public Task<DisplayTask> {
    friend class Task<DisplayTask>; // Allow base Task to invoke protected run()

  public:
    DisplayTask(const uint8_t task_core);
    ~DisplayTask();

    void enqueueEvent(Event event);

  protected:
    void run();

  private:
    SemaphoreHandle_t mutex_;
    void eventHandler(Event event) override;

    xQueueHandle display_event_queue = xQueueCreate(2, sizeof(Event));

    void displayWiFiConnectQR();
    void displayUpdateFirmwareQR();
};
#else

class DisplayTask {};

#endif
