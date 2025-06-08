import esphome.codegen as cg
from esphome.components import text_sensor
from esphome.const import ICON_RESTART, ENTITY_CATEGORY_DIAGNOSTIC

CODEOWNERS = ["@evotodi"]
DEPENDENCIES = []

wake_reason_ns = cg.esphome_ns.namespace("wake_reason")
WakeReasonComponent = wake_reason_ns.class_(
    "WakeReason", text_sensor.TextSensor, cg.Component
)

CONFIG_SCHEMA = (
    text_sensor.text_sensor_schema(
        WakeReasonComponent,
        icon=ICON_RESTART,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,

    )
)

async def to_code(config):
    var = await text_sensor.new_text_sensor(config)
    await cg.register_component(var, config)
