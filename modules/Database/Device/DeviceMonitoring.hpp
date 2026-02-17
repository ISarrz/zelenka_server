#pragma once
#include <cstddef>
#include <string>

class DeviceMonitoring {
   public:
    size_t id;
    size_t device_id;
    std::string timestamp;
    float temperature;
    float humidity;
    float hydration;
    float pressure;
};