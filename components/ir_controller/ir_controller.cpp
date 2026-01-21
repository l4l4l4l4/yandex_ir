#include "ir_controller.h"
#include <IRremoteESP8266.h>
#include <IRutils.h>

namespace esphome {
namespace ir_controller {

static const char *TAG = "ir_controller";

void IRController::setup() {
  // Инициализация пинов
  pinMode(led_pin_, OUTPUT);
  digitalWrite(led_pin_, HIGH);  // Светодиод выключен по умолчанию (инвертированный режим)

  // Инициализация IR приемника и передатчика
  ir_receiver_ = new IRrecv(ir_rx_pin_, 1024, 50, true);
  ir_sender_ = new IRsend(ir_tx_pin_);
  ir_receiver_->enableIRIn();
  ir_sender_->begin();

  // Подписка на изменение состояния переключателя
  if (learning_mode_switch_) {
    learning_mode_switch_->add_on_state_callback([this](bool state) {
      learning_mode_ = state;
      digitalWrite(led_pin_, state ? LOW : HIGH);  // Инвертированная логика: LOW = включён, HIGH = выключен
      if (state) {
        ir_receiver_->enableIRIn();
      } else {
        ir_receiver_->disableIRIn();
      }
    });
  }
}

void IRController::loop() {
  if (learning_mode_) {
    handle_learning_mode_();
  }
}

void IRController::handle_learning_mode_() {
  if (ir_receiver_->decode(&results_)) {
    decode_and_publish_();
    ir_receiver_->resume();
  }
}

void IRController::decode_and_publish_() {
  // Декодирование протокола и данных с преобразованием String в std::string
  std::string protocol = typeToString(results_.decode_type).c_str();
  std::string decoded_data = resultToHexidecimal(&results_).c_str();

  // Публикация в текстовое поле
  if (decoded_text_sensor_) {
    decoded_text_sensor_->publish_state(protocol + ": " + decoded_data);
  }

  ESP_LOGD(TAG, "IR Decoded: Protocol=%s, Data=%s", protocol.c_str(), decoded_data.c_str());
}

void IRController::send_ir_signal(const std::string &protocol, const std::vector<uint16_t> &raw_data) {
  if (learning_mode_) {
    ESP_LOGW(TAG, "Cannot send IR signal in learning mode");
    return;
  }

  // Отправка RAW данных
  ir_sender_->sendRaw(raw_data.data(), raw_data.size(), 38);  // 38 kHz по умолчанию
  ESP_LOGD(TAG, "IR Sent: Protocol=%s, Raw data size=%d", protocol.c_str(), raw_data.size());
}

}  // namespace ir_controller
}  // namespace esphome