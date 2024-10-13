#include "root_task.h"
#include <Arduino.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3) && !SK_FORCE_UART_STREAM
HWCDC stream_ = HWCDC();
#else
// TODO Check if uartstream works!!
UartStream stream_;
#endif

#if ENABLE_LOGGING
static SerialProtocolPlaintext serial_protocol(stream_);
static SerialProtocolPlaintext *serial_protocol_p = &serial_protocol;

static FreeRTOSAdapter adapter(&serial_protocol, xSemaphoreCreateMutex(), "FreeRTOSAdapter", 1024 * 8, 0, 1);
static FreeRTOSAdapter *adapter_p = &adapter;
#endif

RootTask root_task(0);

void setup() {
    delay(100); // Delay to allow usb to connect before starting stream
    stream_.begin(MONITOR_SPEED);

#if ENABLE_LOGGING
    Logging::setAdapter(adapter_p);
#endif

    root_task.begin();

    // Free up the Arduino loop task
    vTaskDelete(NULL);
}

void loop() {}