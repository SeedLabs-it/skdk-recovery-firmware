#pragma once
#include "FFat.h"
#include <logging.h>

class FatGuard {
  public:
    FatGuard() {
        if (!FFat.begin(true)) {
            LOGE("Failed to mount FFat");
            return;
        }
        LOGD("Mounted FFat");
        mounted_ = true;
    }
    ~FatGuard() {
        if (mounted_) {
            FFat.end();
            LOGD("Unmounted FFat");
        }
    }
    FatGuard(FatGuard const &) = delete;
    FatGuard &operator=(FatGuard const &) = delete;

    bool mounted_ = false;
};