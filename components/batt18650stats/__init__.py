import esphome.codegen as cg
from esphome.components import sensor, binary_sensor, output
from esphome import pins
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = []
DEPENDENCIES = []
CODEOWNERS = ["@evotodi"]
MULTI_CONF = True

CONF_BATT_ID = "batt18650_id"
CONF_ADC_PIN = "adc_pin"
CONF_CAL_PIN = "cal_pin"
CONF_LED_OUTPUT = "led_output"
CONF_CONV_FACTOR = "conversion_factor"
CONF_READS = "reads"
CONF_V_MAX = "v_max"
CONF_V_MIN = "v_min"
CONF_TABLE = "use_table"
CONF_LOW_BATT = "low_charge"

batt18650_ns = cg.esphome_ns.namespace("batt18650")
Batt18650StatsComponent = batt18650_ns.class_("Batt18650StatsComponent", cg.PollingComponent, sensor.Sensor, binary_sensor.BinarySensor)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Batt18650StatsComponent),

            cv.Required(CONF_ADC_PIN): pins.gpio_input_pin_schema,
            cv.Required(CONF_CAL_PIN): pins.gpio_input_pin_schema,

            cv.Optional(CONF_LED_OUTPUT): cv.use_id(output.BinaryOutput),
            cv.Optional(CONF_CONV_FACTOR): cv.float_range(min=0, max=100.0),
            cv.Optional(CONF_READS): cv.int_range(min=0, max=100),
            cv.Optional(CONF_V_MAX): cv.float_range(min=0, max=4095),
            cv.Optional(CONF_V_MIN): cv.float_range(min=0, max=4095),
            cv.Optional(CONF_TABLE): cv.boolean,
            cv.Optional(CONF_LOW_BATT): cv.int_range(min=0, max=90),
        }
    )
    .extend(cv.polling_component_schema("10s"))
)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    adcPin = yield cg.gpio_pin_expression(config[CONF_ADC_PIN])
    cg.add(var.set_adc_pin(adcPin))

    calPin = yield cg.gpio_pin_expression(config[CONF_CAL_PIN])
    cg.add(var.set_cal_pin(calPin))

    if CONF_LED_OUTPUT in config:
        out = yield cg.get_variable(config[CONF_LED_OUTPUT])
        cg.add(var.set_led_output(out))

    if CONF_CONV_FACTOR in config:
        cg.add(var.set_conversion_factor(config[CONF_CONV_FACTOR]))
    if CONF_READS in config:
        cg.add(var.set_reads(config[CONF_READS]))
    if CONF_V_MAX in config:
        cg.add(var.set_max_voltage(config[CONF_V_MAX]))
    if CONF_V_MIN in config:
        cg.add(var.set_min_voltage(config[CONF_V_MIN]))
    if CONF_TABLE in config:
        cg.add(var.set_use_conversion_table(config[CONF_TABLE]))
    if CONF_LOW_BATT in config:
        cg.add(var.set_low_battery_charge(config[CONF_LOW_BATT]))
