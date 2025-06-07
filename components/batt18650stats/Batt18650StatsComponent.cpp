#include "Batt18650StatsComponent.h"

namespace esphome {
    namespace batt18650 {
        static const char *TAG = "batt18650";
        uint32_t global_batt_id = 1911042025ULL;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

        void Batt18650StatsComponent::setup() {
            ESP_LOGI(TAG, "Battery Setup...");
            this->rtc_ = global_preferences->make_preference<double>(global_batt_id);
            global_batt_id++;

            if (!this->_conversionFactorFromYaml) {
                ESP_LOGI(TAG, "Loading conversion factor from flash");
                double convFactor;

                if (this->rtc_.load(&convFactor)) {
                    this->_conversionFactor = convFactor;
                    ESP_LOGI(TAG, "Conversion factor from flash: %f", convFactor);
                } else {
                    ESP_LOGE(TAG, "Unable to load conversion factor from flash. Using conversion factor of %f", this->_conversionFactor);
                }
            }

            ESP_LOGI(TAG, "Setup Battery 1860 Stats...");
            this->_battery = new Battery18650Stats(this->_adcPin->get_pin(), this->_conversionFactor, this->_reads, this->_voltageMax, this->_voltageMin);

            ESP_LOGI(TAG, "Checking for calibration button press...");
            this->checkCalPin();
        }

        void Batt18650StatsComponent::loop() {
        }

        void Batt18650StatsComponent::dump_config() {
            ESP_LOGCONFIG(TAG, "Battery18650StatsComponent:");
            ESP_LOGCONFIG(TAG, "\tAdc Pin: %d", this->_adcPin->get_pin());
            ESP_LOGCONFIG(TAG, "\tCal Pin: %d", this->_calPin->get_pin());
            ESP_LOGCONFIG(TAG, "\tConversion Factor from flash: %s", this->_conversionFactorFromYaml ? "Yes" : "No");
            ESP_LOGCONFIG(TAG, "\tConversion Factor: %.3f", this->_conversionFactor);
            ESP_LOGCONFIG(TAG, "\tReads: %d", this->_reads);
            ESP_LOGCONFIG(TAG, "\tVoltage Max: %.3f", this->_voltageMax);
            ESP_LOGCONFIG(TAG, "\tVoltage Min: %.3f", this->_voltageMin);
            ESP_LOGCONFIG(TAG, "\tUse Conversion Table: %s", this->_useConversionTable ? "yes" : "no");
            ESP_LOGCONFIG(TAG, "\tLow Batt Charge: %d%%", this->_lowBattCharge);
        }

        void Batt18650StatsComponent::update() {
            updateBattery();

            if (this->_voltsSensor != nullptr) {
                this->_voltsSensor->publish_state(this->_volts);
            }

            if (this->_chargeSensor != nullptr) {
                this->_chargeSensor->publish_state(this->_charge);
            }

            if (this->_batteryLowSensor != nullptr) {
                this->_batteryLowSensor->publish_state(this->_batteryLow);
            }
        }

        void Batt18650StatsComponent::updateBattery() {
            ESP_LOGD(TAG, "Update battery...");

            this->_charge = this->_battery->readBatteryChargeLevel(this->_useConversionTable);
            ESP_LOGD(TAG, "Charge: %d", this->_charge);

            ESP_LOGD(TAG, "Raw Analog: %d", this->_battery->getRawAnalogValue());

            this->_volts = this->_battery->getVoltage();
            ESP_LOGD(TAG, "Voltage: %.3f", this->_volts);

            if (this->_charge <= this->_lowBattCharge) {
                this->_batteryLow = true;
            } else {
                this->_batteryLow = false;
            }
            ESP_LOGD(TAG, "Battery low: %s", this->_batteryLow ? "Yes" : "No");
        }

        void Batt18650StatsComponent::checkCalPin() {
            ESP_LOGI(TAG, "Waiting for battery calibration command. Battery MUST be fully charged! Press boot button.");

            unsigned long calCheckEndMillis = millis() + CAL_CHECK_MSECS;
            unsigned long previousMillisHB = 0;
            int lastSteadyState = LOW; // the previous steady state from the input pin
            int lastFlickerableState = LOW; // the previous flickerable state from the input pin
            unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
            int currentState;
            bool calBtnPressed = false;
            bool ledState = false;

            while (millis() < calCheckEndMillis) {
                // read the state of the switch/button:
                currentState = digitalRead(this->_calPin->get_pin());

                if (this->_ledOutput != nullptr) {
                    if(millis() - previousMillisHB > 150 || previousMillisHB == 0) {
                        ledState = !ledState;
                        this->_ledOutput->set_state(ledState);
                        previousMillisHB = millis();
                    }
                }

                // check to see if you just pressed the button
                // (i.e. the input went from LOW to HIGH), and you've waited long enough
                // since the last press to ignore any noise:

                // If the switch/button changed, due to noise or pressing:
                if (currentState != lastFlickerableState) {
                    // reset the debouncing timer
                    lastDebounceTime = millis();
                    // save the last flickerable state
                    lastFlickerableState = currentState;
                }

                if ((millis() - lastDebounceTime) > CAL_DEBOUNCE_TIME) {
                    // whatever the reading is at, it's been there for longer than the debounced
                    // delay, so take it as the actual current state:

                    // if the button state has changed:
                    if (lastSteadyState == HIGH && currentState == LOW) {
                        ESP_LOGI(TAG, "Cal button pressed");
                        calBtnPressed = true;
                        break;
                    } else if (lastSteadyState == LOW && currentState == LOW) {
                        ESP_LOGI(TAG, "Cal button held");
                        calBtnPressed = true;
                        break;
                    }

                    // save the last steady state
                    lastSteadyState = currentState;
                }
            }
            if (!calBtnPressed) {
                ESP_LOGI(TAG, "Battery calibration command timeout");
            } else {
                ESP_LOGI(TAG, "Performing battery calibration");
                calibrateBatteryAdc();
            }

            if (this->_ledOutput != nullptr) {
                this->_ledOutput->set_state(true);
            }
        }

        void Batt18650StatsComponent::calibrateBatteryAdc() {
            double bLow, bHigh;
            ESP_LOGW(TAG, "Battery calibration start...");

            if (this->_ledOutput != nullptr) {
                this->_ledOutput->set_state(true);
                delay(250);
                this->_ledOutput->set_state(false);
                delay(250);
            }

            bLow = constrain(this->_voltageMin, 0, 5.0);
            bHigh = constrain(this->_voltageMax, 0, 5.0);
            ESP_LOGW(TAG, "Volts Low = %f High = %f", bLow, bHigh);

            int adc = this->_battery->pinRead();
            double volts = this->_battery->readBatteryVolts();

            ESP_LOGW(TAG, "ADC Counts = %d Volts = %f", adc, volts);
            double convFactor = (bHigh * BATT_V_MULTIPLIER) / adc;
            ESP_LOGW(TAG, "Conversion factor = %f", convFactor);

            if (this->_ledOutput != nullptr) {
                this->_ledOutput->set_state(true);
                delay(250);
                this->_ledOutput->set_state(false);
                delay(250);
            }

            ESP_LOGW(TAG, "Validating the conversion factor...");
            this->_battery = new Battery18650Stats(this->_adcPin->get_pin(), convFactor, this->_reads, bHigh, bLow);
            adc = this->_battery->pinRead();
            volts = this->_battery->readBatteryVolts();
            ESP_LOGW(TAG, "ADC Counts = %d Volts = %f", adc, volts);

            ESP_LOGW(TAG, "Saving conversion factor to flash");
            this->rtc_.save(&convFactor);

            if (this->_ledOutput != nullptr) {
                this->_ledOutput->set_state(true);
                delay(250);
                this->_ledOutput->set_state(false);
                delay(250);
            }
        }
    } //namespace batt18650
} //namespace esphome
