#pragma once
#include "LedManagerTask.h"

class DummyLedManagerTask : public LedManagerTask {
public:
    DummyLedManagerTask() {}
    void begin(EvseManager &) {}
    void setup() {}
    unsigned long loop(MicroTasks::WakeReason) { return MicroTask.Infinate; }
    void clear() {}
    void test() {}
    void setWifiMode(bool, bool) {}
    int getButtonPressed() { return LOW; }
    void setBrightness(uint8_t) {}
};