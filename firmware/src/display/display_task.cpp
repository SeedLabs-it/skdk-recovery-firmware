#include "display_task.h"

DisplayTask::DisplayTask(const uint8_t task_core) : Task("DisplayTask", 1024 * 16, 1, task_core) {
    mutex_ = xSemaphoreCreateMutex();
    assert(mutex_ != NULL);
}
DisplayTask::~DisplayTask() { vSemaphoreDelete(mutex_); }

const uint8_t QR_CODE_SIZE = 90;

const uint8_t QR_X = (TFT_WIDTH / 2) - (QR_CODE_SIZE / 2);
const uint8_t QR_Y = (TFT_WIDTH / 2) - (QR_CODE_SIZE / 2) - 10;

const uint8_t TEXT_START_X = TFT_WIDTH / 2;
const uint8_t TEXT_START_Y = QR_Y + (QR_CODE_SIZE / 2) * 2 + 10;

void DisplayTask::run() {
    LOG("DisplayTask::run");

    lv_skdk_create();

    LGFX *lcd = lv_skdk_get_lcd();

    lcd->setTextColor(TFT_WHITE);
    lcd->setTextSize(1);

    displayWiFiConnectQR();

    while (1) {
        {
            if (xQueueReceive(display_event_queue, &event, 0)) {
                eventHandler(event);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void DisplayTask::displayWiFiConnectQR() {
    LGFX *lcd = lv_skdk_get_lcd();
    lcd->fillScreen(TFT_BLACK);

    char url[128];
    std::string mac_address = std::string(WiFi.macAddress().c_str());
    mac_address.erase(std::remove_if(mac_address.begin(), mac_address.end(), [](char c) { return c == ':'; }),
                      mac_address.end());

    char ssid[64];
    snprintf(ssid, sizeof(ssid), "SKDK_%s", mac_address.substr(mac_address.length() - 6).c_str());

    snprintf(url, sizeof(url), "WIFI:T:WPA;S:%s;P:%s;H:;;", ssid, "12345678");
    lcd->qrcode(url, QR_X, QR_Y, QR_CODE_SIZE, 1);

    char text[64];
    snprintf(text, sizeof(text), "RECOVERY");

    uint8_t text_width = lcd->textWidth(text);
    lcd->setTextColor(TFT_ORANGE);
    lcd->setCursor(TEXT_START_X - text_width / 2, 20);
    lcd->print(text);

    snprintf(text, sizeof(text), "Connect to WiFi");
    text_width = lcd->textWidth(text);
    lcd->setTextColor(TFT_WHITE);
    lcd->setCursor(TEXT_START_X - text_width / 2, QR_Y - 16);
    lcd->print(text);

    snprintf(text, sizeof(text), "SSID: %s", ssid);
    text_width = lcd->textWidth(text);
    lcd->setCursor(TEXT_START_X - text_width / 2, TEXT_START_Y);
    lcd->print(text);

    snprintf(text, sizeof(text), "PASSWORD: 12345678");
    text_width = lcd->textWidth(text);
    lcd->setCursor(TEXT_START_X - text_width / 2, TEXT_START_Y + 10);
    lcd->print(text);
}

void DisplayTask::displayUpdateFirmwareQR() {
    LGFX *lcd = lv_skdk_get_lcd();
    lcd->fillScreen(TFT_BLACK);

    char url[128];
    snprintf(url, sizeof(url), "http://%s/update", WiFi.softAPIP().toString().c_str());
    lcd->qrcode(url, QR_X, QR_Y, QR_CODE_SIZE, 1);

    char text[64];
    snprintf(text, sizeof(text), "Update/Install Firmware");

    uint8_t text_width = lcd->textWidth(text);
    lcd->setCursor(TEXT_START_X - text_width / 2, QR_Y - 16);
    lcd->print(text);

    snprintf(text, sizeof(text), "GOTO URL");
    text_width = lcd->textWidth(text);
    lcd->setCursor(TEXT_START_X - text_width / 2, TEXT_START_Y);
    lcd->print(text);

    snprintf(text, sizeof(text), "%s", url);
    text_width = lcd->textWidth(text);
    lcd->setCursor(TEXT_START_X - text_width / 2, TEXT_START_Y + 10);
    lcd->print(text);
}

void DisplayTask::enqueueEvent(Event event) { xQueueSend(display_event_queue, &event, portMAX_DELAY); }

void DisplayTask::eventHandler(Event event) {
    switch (event.type) {
    case SK_WIFI_CLIENT_CONNECTED:
        LOGV(LOG_LEVEL_DEBUG, "SK_WIFI_CLIENT_CONNECTED IN DISPLAYTASK");
        displayUpdateFirmwareQR();
        // display qr to update firmware url
        break;
    case SK_WIFI_CLIENT_DISCONNECTED:
        LOGV(LOG_LEVEL_DEBUG, "SK_WIFI_CLIENT_DISCONNECTED IN DISPLAYTASK");
        // display connect to wifi
        break;
    default:
        LOGV(LOG_LEVEL_DEBUG, "Unknown event type: %d", event.type);
        break;
    }
}