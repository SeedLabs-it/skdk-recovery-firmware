#include "wifi_task.h"

WiFiTask::WiFiTask(const uint8_t task_core) : Task("WiFiTask", 4096, 1, task_core) {}
WiFiTask::~WiFiTask() {}

void WiFiTask::run() {
    std::string mac_address = std::string(WiFi.macAddress().c_str());
    mac_address.erase(std::remove_if(mac_address.begin(), mac_address.end(), [](char c) { return c == ':'; }),
                      mac_address.end());

    char ssid[64];
    snprintf(ssid, sizeof(ssid), "SKDK_%s", mac_address.substr(mac_address.length() - 6).c_str());
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.softAP(ssid, "12345678");

    server_ = new WebServer(80);
    ElegantOTA.begin(server_);

#if SK_ELEGANTOTA_PRO
    char release_version[64];
    snprintf(release_version, sizeof(release_version), "RECOVERY-%s",
             RELEASE_VERSION ? RELEASE_VERSION : "RECOVERY DEV");

    ElegantOTA.setID(ssid);
    ElegantOTA.setFWVersion(release_version);
    ElegantOTA.setFirmwareMode(true);
    ElegantOTA.setFilesystemMode(false);
    ElegantOTA.setTitle("SKDK - Update");
    ElegantOTA.setAutoReboot(true);
#endif

    server_->on("/", [this]() { this->handleRoot(); });
    server_->begin();

    while (1) {
        server_->handleClient();
        delay(5);
    }
}

void WiFiTask::handleRoot() {
    server_->sendHeader("Location", "/update", true);
    server_->send(301, "text/plain", "");
}

void WiFiTask::eventHandler(Event event) {
    switch (event.type) {
    default:
        LOGV(LOG_LEVEL_DEBUG, "Unhandled event in WiFiTask, event type: %d", event.type);
        break;
    }
}