#include "Ds18PoweredComponent.h"

namespace esphome {
    namespace ds18powered {
        static const char *TAG = "ds18powered";

        void Ds18PoweredComponent::setup() {
            ESP_LOGD(TAG, "Setting up Dallas temperature sensor...");
            this->_pwrOutput->set_state(true);
            delay(10);
            this->_oneWire = new OneWire(this->_owPin->get_pin());
            this->_sensors.setOneWire(this->_oneWire);

            ESP_LOGD(TAG, "Locating devices...");
            this->_sensors.begin();
            ESP_LOGD(TAG, "Found %d devices", this->_sensors.getDeviceCount());

            if (!this->_sensors.getAddress(this->_tempSensor, 0)) {
                ESP_LOGD(TAG, "Unable to find address for Device 0");
                this->_foundSensor = false;
                return;
            }

            ESP_LOGD(TAG, "Device 0 Address: %s", getDs18b20AddressName().c_str());
            this->_sensors.setResolution(this->_tempSensor, this->_dsResolution);
            ESP_LOGD(TAG, "Device 0 Resolution: %d", this->_sensors.getResolution(this->_tempSensor));
        }

        void Ds18PoweredComponent::loop() {
            PollingComponent::loop();
        }

        void Ds18PoweredComponent::update() {
            if (this->_temp_sensor != nullptr) {
                ESP_LOGD(TAG, "Requesting temperatures...");
                this->_sensors.requestTemperatures(); // Send the command to get temperatures
                getTemperature(this->_tempSensor);
                this->_temp_sensor->publish_state(this->_oTemp);
            }
        }

        void Ds18PoweredComponent::dump_config() {
            ESP_LOGCONFIG(TAG, "DS18 Powered Component:");
            ESP_LOGCONFIG(TAG, "\tOw Pin: %d", this->_owPin->get_pin());
            ESP_LOGCONFIG(TAG, "\tResolution Requested: %u bits", this->_dsResolution);
            ESP_LOGCONFIG(TAG, "\tDevice 0 Address: %s", getDs18b20AddressName().c_str());
            LOG_UPDATE_INTERVAL(this);
        }

        void Ds18PoweredComponent::set_ow_pin(InternalGPIOPin *owPin) {
            this->_owPin = owPin;
        }

        const std::string &Ds18PoweredComponent::getDs18b20AddressName() {
            if (this->_addressName.empty()) {
                this->_addressName = std::string("0x");
                for (int i = 0; i < 8; i++) {
                    this->_addressName += format_hex(this->_tempSensor[i]);
                }
            }
            return this->_addressName;
        }

        void Ds18PoweredComponent::getTemperature(DeviceAddress deviceAddress) {
            this->_oTemp = this->_sensors.getTempC(deviceAddress);
            if (this->_oTemp == DEVICE_DISCONNECTED_C) {
                ESP_LOGE(TAG, "Error: Could not read temperature data");
                ESP_LOGE(TAG, "Temp Value: %.2f", this->_oTemp);
            }
        }
    }
}
