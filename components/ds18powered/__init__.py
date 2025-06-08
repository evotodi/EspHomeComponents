import esphome.codegen as cg
from esphome.components import sensor, output
from esphome import pins
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_RESOLUTION
)

AUTO_LOAD = []
DEPENDENCIES = []
CODEOWNERS = ["@evotodi"]
MULTI_CONF = True

CONF_DS18_ID = "ds18powered_id"
CONF_OW_PIN = "ow_pin"
CONF_PWR_OUTPUT = "pwr_output"

ds18powered_ns = cg.esphome_ns.namespace("ds18powered")
Ds18PoweredComponent = ds18powered_ns.class_("Ds18PoweredComponent", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Ds18PoweredComponent),

            cv.Required(CONF_OW_PIN): pins.gpio_input_pin_schema,
            cv.Required(CONF_PWR_OUTPUT): cv.use_id(output.BinaryOutput),

            cv.Optional(CONF_RESOLUTION, default=12): cv.int_range(min=9, max=12),
        }
    )
    .extend(cv.polling_component_schema("10s"))
)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    owPin = yield cg.gpio_pin_expression(config[CONF_OW_PIN])
    cg.add(var.set_ow_pin(owPin))

    out = yield cg.get_variable(config[CONF_PWR_OUTPUT])
    cg.add(var.set_pwr_output(out))

    if CONF_RESOLUTION in config:
        cg.add(var.set_ow_resolution(config[CONF_RESOLUTION]))
