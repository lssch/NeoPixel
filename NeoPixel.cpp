//
// Created by Lars Schwarz on 13.10.2023.
//

#include <vector>
#include "NeoPixel.h"

NeoPixel::Pixel::Pixel() {
  off();
}

void NeoPixel::Pixel::off() {
  red = 0;
  green = 0;
  blue = 0;
  brightness = 100;
}

void NeoPixel::Pixel::set_color(uint8_t red_, uint8_t green_, uint8_t blue_) {
  red = red_;
  green = green_;
  blue = blue_;
}

void NeoPixel::Pixel::set_brightness(uint8_t brightness_) {
  brightness = brightness_;
}

NeoPixel::Controller::Controller(TIM_HandleTypeDef *htim_, uint8_t channel_, type_e type_, uint8_t number_of_pixels) {
  htim = htim_;
  channel = channel_;
  type = type_;
  add_pixel(number_of_pixels);
}

void NeoPixel::Controller::add_pixel(uint8_t number_of_pixels) {
  NeoPixel::Pixel pixel;
  for (uint8_t i = 0; i < number_of_pixels; ++i)
    pixels.push_back(pixel);
}

void NeoPixel::Controller::off() {
  for (auto &pixel: pixels)
    pixel.off();
}

void NeoPixel::Controller::set_color(uint8_t red, uint8_t green, uint8_t blue) {
  for (auto &pixel: pixels)
    pixel.set_color(red, green, blue);
}

void NeoPixel::Controller::set_brightness(uint8_t brightness) {
  for (auto &pixel: pixels)
    pixel.set_brightness(brightness);
}

void NeoPixel::Controller::update() {
  while (dma_active);
  dma_buffer.clear();
  uint32_t color;
  for (auto &pixel: pixels) {
    color = (((pixel.green*pixel.brightness/100) << 16)
            | ((pixel.red*pixel.brightness/100) << 8)
            | (pixel.blue*pixel.brightness/100));
    for (int i = 23; i >=0 ; i--)
      dma_buffer.push_back((color&(1<<i)) ? LOGIC_ONE : LOGIC_ZERO);
  }
  dma_buffer.push_back(0);
  HAL_TIM_PWM_Start_DMA(htim, channel, dma_buffer.data(), dma_buffer.size());
  dma_active = true;
}

void NeoPixel::Controller::dma_finished() {
  HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
  dma_active = false;
}
