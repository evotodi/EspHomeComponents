#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/components/sensor/sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

namespace esphome {
    namespace ds18powered {
        class Ds18PoweredComponent : public PollingComponent, public sensor::Sensor {
        public:
            void set_temp_sensor(sensor::Sensor *temp_sensor) { _temp_sensor = temp_sensor; }
            void set_ow_resolution(uint8_t resolution) { this->_dsResolution = resolution; }
            void set_ow_pin(InternalGPIOPin *owPin);
            void set_pwr_output(output::BinaryOutput *output) { this->_pwrOutput = output; }

            void setup() override;
            void loop() override;
            void update() override;
            void dump_config() override;

        protected:
            sensor::Sensor *_temp_sensor;
            OneWire *_oneWire;
            DallasTemperature _sensors;
            DeviceAddress _tempSensor;
            std::string _addressName;
            uint8_t _dsResolution = 12;
            InternalGPIOPin *_owPin;
            output::BinaryOutput *_pwrOutput{nullptr};

            float _oTemp = -127.0;
            bool _foundSensor = true;

            // Helper to create (and cache) the name for this sensor
            const std::string &getDs18b20AddressName();
            void getTemperature(DeviceAddress deviceAddress);
        };
    } //namespace ds18powered
} //namespace esphome
