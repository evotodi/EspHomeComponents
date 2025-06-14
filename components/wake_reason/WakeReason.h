#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esp_sleep.h"

namespace esphome {
    namespace wake_reason {
        class WakeReason : public text_sensor::TextSensor, public Component {
        public:
            void setup() override;
            void dump_config() override;
            float get_setup_priority() const override;
            std::string unique_id() override;
        };
    } // namespace wake_reason
} // namespace esphome
