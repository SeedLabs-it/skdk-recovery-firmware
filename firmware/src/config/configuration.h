#pragma once
#include "fat_guard.h"

// TODO move to seperate shared lib between barebones and release firmware

const uint32_t PERSISTENT_CONFIGURATION_VERSION = 2;
const uint32_t SETTINGS_VERSION = 1;

class Configuration {
  public:
    Configuration();
    ~Configuration();

    bool load();
    bool save();
}