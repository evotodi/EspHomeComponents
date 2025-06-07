import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    UNIT_VOLT,
    ICON_BATTERY,
    ICON_PERCENT,
    UNIT_PERCENT,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_VOLUME,
)

from . import CONF_BATT_ID, Batt18650StatsComponent

CODEOWNERS = ["@evotodi"]
DEPENDENCIES = ["batt18650stats"]

CONF_VOLT_SENSOR = "voltage_sensor"
CONF_CHARGE_SENSOR = "charge_sensor"


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_BATT_ID): cv.use_id(Batt18650StatsComponent),

        cv.Optional(CONF_VOLT_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=2,
            icon=ICON_BATTERY,
            device_class=DEVICE_CLASS_BATTERY,
            state_class=STATE_CLASS_MEASUREMENT
        ),

        cv.Optional(CONF_CHARGE_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=2,
            icon=ICON_PERCENT,
            device_class=DEVICE_CLASS_VOLUME,
            state_class=STATE_CLASS_MEASUREMENT
        ),
    }
)

def to_code(config):
    hub = yield cg.get_variable(config[CONF_BATT_ID])

    if volt_config := config.get(CONF_VOLT_SENSOR):
        sens = yield sensor.new_sensor(volt_config)
        cg.add(hub.set_volts_sensor(sens))

    if charge_config := config.get(CONF_CHARGE_SENSOR):
        sens = yield sensor.new_sensor(charge_config)
        cg.add(hub.set_charge_sensor(sens))
