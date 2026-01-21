#pragma once

#include "esphome.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>

namespace esphome {
namespace ir_controller {

class IRController : public Component {
 public:
  void setup() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Конфигурация пинов и компонентов
  void set_ir_rx_pin(uint8_t pin) { ir_rx_pin_ = pin; }
  void set_ir_tx_pin(uint8_t pin) { ir_tx_pin_ = pin; }
  void set_led_pin(uint8_t pin) { led_pin_ = pin; }
  void set_learning_mode_switch(switch_::Switch *sw) { learning_mode_switch_ = sw; }
  void set_decoded_text_sensor(text_sensor::TextSensor *sensor) { decoded_text_sensor_ = sensor; }

  // Метод для отправки ИК-сигнала из сервиса HA
  void send_ir_signal(const std::string &protocol, const std::vector<uint16_t> &raw_data);

 protected:
  uint8_t ir_rx_pin_;
  uint8_t ir_tx_pin_;
  uint8_t led_pin_;
  switch_::Switch *learning_mode_switch_;
  text_sensor::TextSensor *decoded_text_sensor_;

  IRrecv *ir_receiver_;
  IRsend *ir_sender_;
  decode_results results_;

  bool learning_mode_ = false;

  void handle_learning_mode_();
  void decode_and_publish_();
};

}  // namespace ir_controller
}  // namespace esphome