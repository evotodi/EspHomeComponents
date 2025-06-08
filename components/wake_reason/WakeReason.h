#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"

// extern int esp_sleep_get_wakeup_cause();

namespace esphome {
    namespace wake_reason {
        class WakeReason : public text_sensor::TextSensor, public Component {
        public:
            void setup() override;
            void dump_config() override;

        protected:
        };
    } // namespace empty_text_sensor
} // namespace esphome
