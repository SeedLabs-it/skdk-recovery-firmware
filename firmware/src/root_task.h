#pragma once
#include "display/display_task.h"
#include "logging.h"
#include "logging/adapters/freertos/free_rtos_adapter.h"
#include "network/wifi_task.h"
#include "task.h"
#include <FastLED.h>

class RootTask : public Task<RootTask> { /*  */
    friend class Task<RootTask>;         // Allow base Task to invoke protected run()
  public:
    RootTask(const BaseType_t coreId);
    virtual ~RootTask();

  protected:
    void run();

  private:
#if defined(CONFIG_IDF_TARGET_ESP32S3) && !SK_FORCE_UART_STREAM
    HWCDC stream_;
#else
    UartStream stream_;
#endif
    void eventHandler(Event event) override;

    static void WiFiEventHandler(WiFiEvent_t event);
};