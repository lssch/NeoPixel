//
// Created by Lars Schwarz on 13.10.2023.
//

#include <vector>
#include <valarray>
#include "NeoPixel.h"

NeoPixel::Pixel::Pixel() {
  off();
  set_brightness(100);
}

void NeoPixel::Pixel::off() {
  _color.red = 0;
  _color.green = 0;
  _color.blue = 0;
}

void NeoPixel::Pixel::set_color(uint8_t red, uint8_t green, uint8_t blue) {
  _color.red = red;
  _color.green = green;
  _color.blue = blue;
}

void NeoPixel::Pixel::set_brightness(uint8_t brightness) {
  _color.brightness = brightness;
}

NeoPixel::Color NeoPixel::Pixel::getColor() {
  return _color;
}

NeoPixel::Group::Group(NeoPixel::Controller* controller, std::initializer_list<uint8_t> indexes) {
  for (const auto &item: indexes) {
    _pixels.push_back(controller->get_pixel(item));
  }
}

NeoPixel::Group::Group(NeoPixel::Controller* controller, uint8_t start, uint8_t end) {
  for (int i = start; i < end; ++i) {
    _pixels.push_back(controller->get_pixel(i));
  }
}

void NeoPixel::Group::off() {
  for (auto &pixel: _pixels)
    pixel->off();
}

void NeoPixel::Group::set_color(uint8_t red, uint8_t green, uint8_t blue) {
  for (auto &pixel: _pixels)
    pixel->set_color(red, green, blue);
}

void NeoPixel::Group::set_brightness(uint8_t brightness) {
  for (auto &pixel: _pixels)
    pixel->set_brightness(brightness);
}

NeoPixel::Controller::Controller(NeoPixel::Controller::timer_t timer, NeoPixel::Controller::type_e type,
                                 uint8_t number_of_pixels) : _timer(timer), _type(type) {
  for (int i = 0; i < number_of_pixels; ++i) {
    _pixels.push_back(new NeoPixel::Pixel);
  }
}

NeoPixel::Pixel* NeoPixel::Controller::get_pixel(uint8_t position) {
  return _pixels.at(position);
}

void NeoPixel::Controller::off() {
  for (auto &pixel: _pixels)
    pixel->off();
}

void NeoPixel::Controller::set_color(uint8_t red, uint8_t green, uint8_t blue) {
  for (auto &pixel: _pixels)
    pixel->set_color(red, green, blue);
}

void NeoPixel::Controller::set_brightness(uint8_t brightness) {
  for (auto &pixel: _pixels)
    pixel->set_brightness(brightness);
}

void NeoPixel::Controller::update() {
  while (_dma_active);
  _dma_buffer.clear();
  uint32_t color;
  for (auto &pixel: _pixels) {
    color = (((pixel->getColor().green * pixel->getColor().brightness / 100) << 16)
            | ((pixel->getColor().red * pixel->getColor().brightness / 100) << 8)
            | (pixel->getColor().blue * pixel->getColor().brightness / 100));
    for (int i = 23; i >=0 ; i--)
      _dma_buffer.push_back((color & (1 << i)) ? LOGIC_ONE : LOGIC_ZERO);
  }
  _dma_buffer.push_back(0);
  HAL_TIM_PWM_Start_DMA(_timer.htim, _timer.channel, _dma_buffer.data(), _dma_buffer.size());
  _dma_active = true;
}

void NeoPixel::Controller::dma_finished() {
  HAL_TIM_PWM_Stop_DMA(_timer.htim, _timer.channel);
  _dma_active = false;
}