import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    STATE_CLASS_MEASUREMENT,
    ICON_THERMOMETER,
    DEVICE_CLASS_TEMPERATURE,
)

from . import CONF_DS18_ID, Ds18PoweredComponent

CODEOWNERS = ["@evotodi"]
DEPENDENCIES = ["ds18powered"]

CONF_TEMP_SENSOR = "temp_sensor"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_DS18_ID): cv.use_id(Ds18PoweredComponent),

        cv.Optional(CONF_TEMP_SENSOR): sensor.sensor_schema(
            accuracy_decimals=2,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT
        ),
    }
)

def to_code(config):
    hub = yield cg.get_variable(config[CONF_DS18_ID])

    if temp_config := config.get(CONF_TEMP_SENSOR):
        sens = yield sensor.new_sensor(temp_config)
        cg.add(hub.set_temp_sensor(sens))
