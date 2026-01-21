import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch, text_sensor
from esphome.const import CONF_ID, CONF_PIN

DEPENDENCIES = ['switch', 'text_sensor']
AUTO_LOAD = ['switch', 'text_sensor']

ir_controller_ns = cg.esphome_ns.namespace('ir_controller')
IRController = ir_controller_ns.class_('IRController', cg.Component)

CONF_IR_RX_PIN = 'ir_rx_pin'
CONF_IR_TX_PIN = 'ir_tx_pin'
CONF_LED_PIN = 'led_pin'
CONF_LEARNING_MODE_SWITCH = 'learning_mode_switch'
CONF_DECODED_TEXT_SENSOR = 'decoded_text_sensor'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(IRController),
    cv.Required(CONF_IR_RX_PIN): cv.int_,
    cv.Required(CONF_IR_TX_PIN): cv.int_,
    cv.Required(CONF_LED_PIN): cv.int_,
    cv.Required(CONF_LEARNING_MODE_SWITCH): cv.use_id(switch.Switch),
    cv.Required(CONF_DECODED_TEXT_SENSOR): cv.use_id(text_sensor.TextSensor),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # Настройка пинов
    cg.add(var.set_ir_rx_pin(config[CONF_IR_RX_PIN]))
    cg.add(var.set_ir_tx_pin(config[CONF_IR_TX_PIN]))
    cg.add(var.set_led_pin(config[CONF_LED_PIN]))

    # Настройка переключателя и текстового сенсора
    learning_switch = await cg.get_variable(config[CONF_LEARNING_MODE_SWITCH])
    cg.add(var.set_learning_mode_switch(learning_switch))
    decoded_sensor = await cg.get_variable(config[CONF_DECODED_TEXT_SENSOR])
    cg.add(var.set_decoded_text_sensor(decoded_sensor))

    # Добавление библиотеки IRremoteESP8266
    cg.add_library("IRremoteESP8266", "2.8.6")