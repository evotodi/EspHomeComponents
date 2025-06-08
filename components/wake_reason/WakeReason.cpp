#include "WakeReason.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace wake_reason {
        static const char *TAG = "wake_reason";

        void WakeReason::setup() {
            int reason;
            reason = esp_sleep_get_wakeup_cause();
            ESP_LOGD(TAG, "Wakeup reason value: %d", reason);
            switch (reason) {
                case 1:
                    ESP_LOGD(TAG, "Wakeup reason: All");
                    this->publish_state("All");
                    break;
                case 2:
                    ESP_LOGD(TAG, "Wakeup reason: ETX0");
                    this->publish_state("EXT0");
                    break;
                case 3:
                    ESP_LOGD(TAG, "Wakeup reason: EXT1");
                    this->publish_state("EXT1");
                    break;
                case 4:
                    ESP_LOGD(TAG, "Wakeup reason: Timer");
                    this->publish_state("Timer");
                    break;
                case 5:
                    ESP_LOGD(TAG, "Wakeup reason: Touchpad");
                    this->publish_state("Touchpad");
                    break;
                case 6:
                    ESP_LOGD(TAG, "Wakeup reason: ULP");
                    this->publish_state("ULP");
                    break;
                case 7:
                    ESP_LOGD(TAG, "Wakeup reason: GPIO");
                    this->publish_state("GPIO");
                    break;
                case 8:
                    ESP_LOGD(TAG, "Wakeup reason: UART");
                    this->publish_state("UART");
                    break;
                case 9:
                    ESP_LOGD(TAG, "Wakeup reason: WIFI");
                    this->publish_state("WIFI");
                    break;
                case 10:
                    ESP_LOGD(TAG, "Wakeup reason: CPU Init");
                    this->publish_state("CPU Init");
                    break;
                case 11:
                    ESP_LOGD(TAG, "Wakeup reason: CPU Crash");
                    this->publish_state("CPU Crash");
                    break;
                case 12:
                    ESP_LOGD(TAG, "Wakeup reason: BT");
                    this->publish_state("BT");
                    break;
                default:
                    ESP_LOGD(TAG, "Wakeup reason: Unknown");
                    this->publish_state("Unknown");
                    break;
            }
        }

        void WakeReason::dump_config() {
            LOG_TEXT_SENSOR("", "Wake Reason Text Sensor", this);
        }

        float WakeReason::get_setup_priority() const {
            return setup_priority::DATA;
        }

        std::string WakeReason::unique_id() {
            return get_mac_address() + "-wake-reason";
        }
    }
}