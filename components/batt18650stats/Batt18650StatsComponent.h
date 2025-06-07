#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/preferences.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/output/binary_output.h"

#include "Battery18650Stats.h"

#define CAL_DEBOUNCE_TIME 50
#define CAL_CHECK_MSECS 3000
#define BATT_V_MULTIPLIER 1000

namespace esphome {
    namespace batt18650 {
        extern uint32_t global_batt_id;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

        class Batt18650StatsComponent : public PollingComponent, public sensor::Sensor, public binary_sensor::BinarySensor {
        public:
            void set_volts_sensor(sensor::Sensor *voltsSensor) { _voltsSensor = voltsSensor; }
            void set_charge_sensor(sensor::Sensor *chargeSensor) { _chargeSensor = chargeSensor; }
            void set_battery_low_sensor(binary_sensor::BinarySensor *batteryLowSensor) { _batteryLowSensor = batteryLowSensor; }

            void set_adc_pin(InternalGPIOPin *adcPin) { this->_adcPin = adcPin; }
            void set_cal_pin(InternalGPIOPin *calPin) { this->_calPin = calPin; }
            void set_led_output(output::BinaryOutput *output) { this->_ledOutput = output; }
            void set_conversion_factor(double conversionFactor) { this->_conversionFactor = conversionFactor; this->_conversionFactorFromYaml = true; }
            void set_reads(int reads) { this->_reads = reads; };
            void set_max_voltage(float maxVoltage) { this->_voltageMax = maxVoltage; }
            void set_min_voltage(float minVoltage) { this->_voltageMin = minVoltage; }
            void set_use_conversion_table(bool useConversionTable) { this->_useConversionTable = useConversionTable; }
            void set_low_battery_charge(int lowBatteryCharge) { this->_lowBattCharge = lowBatteryCharge; }

            void setup() override;
            void loop() override;
            void update() override;
            void dump_config() override;

        protected:
            void updateBattery();
            void checkCalPin();
            void calibrateBatteryAdc();

            ESPPreferenceObject rtc_;
            Battery18650Stats *_battery;
            output::BinaryOutput *_ledOutput{nullptr};
            sensor::Sensor *_voltsSensor;
            sensor::Sensor *_chargeSensor;
            binary_sensor::BinarySensor *_batteryLowSensor;

            InternalGPIOPin *_adcPin;
            InternalGPIOPin *_calPin;
            double _conversionFactor = DEFAULT_CONVERSION_FACTOR;
            bool _conversionFactorFromYaml = false;
            int _reads = DEFAULT_READS;
            float _voltageMax = DEFAULT_MAX_VOLTAGE;
            float _voltageMin = DEFAULT_MIN_VOLTAGE;
            bool _useConversionTable = true;
            int _lowBattCharge = 10;

            double _volts = 0;
            int _charge = 0;
            bool _batteryLow = false;
        };
    } //namespace batt18650
} //namespace esphome
