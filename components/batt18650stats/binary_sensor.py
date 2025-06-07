import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    ICON_POWER,
    DEVICE_CLASS_PROBLEM,
)

from . import CONF_BATT_ID, Batt18650StatsComponent

CODEOWNERS = ["@evotodi"]
DEPENDENCIES = ["batt18650stats"]

CONF_LOW_SENSOR = "batt_low_sensor"


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_BATT_ID): cv.use_id(Batt18650StatsComponent),

        cv.Optional(CONF_LOW_SENSOR): binary_sensor.binary_sensor_schema(
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_PROBLEM,
        )
    }
)

def to_code(config):
    hub = yield cg.get_variable(config[CONF_BATT_ID])

    if low_config := config.get(CONF_LOW_SENSOR):
        sens = yield binary_sensor.new_binary_sensor(low_config)
        cg.add(hub.set_battery_low_sensor(sens))
