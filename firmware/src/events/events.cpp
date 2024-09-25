#include "events/events.h"

QueueHandle_t event_queue = xQueueCreate(10, sizeof(Event));