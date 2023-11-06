//
// Created by Lars Schwarz on 13.10.2023.
//


// TODO: std::array<LedClass, numLedds> leds

#include <vector>
#include "NeoPixel.h"

NeoPixel::Pixel::Pixel() {
  off();
  set_brightness(100);
}

void NeoPixel::Pixel::off() {
  color.red = 0;
  color.green = 0;
  color.blue = 0;
}

void NeoPixel::Pixel::set_color(uint8_t red, uint8_t green, uint8_t blue) {
  color.red = red;
  color.green = green;
  color.blue = blue;
}

void NeoPixel::Pixel::set_brightness(uint8_t brightness) {
  color.brightness = brightness;
}

NeoPixel::Pixel::color_t NeoPixel::Pixel::getColor() {
  return color;
}

NeoPixel::Group::Group(std::initializer_list<Pixel *> pixels_): pixels(pixels_) {}

NeoPixel::Group::Group(NeoPixel::Pixel *pixels_, uint8_t number_of_pixels) {
  for (int i = 0; i < number_of_pixels; ++i) {
    pixels.push_back((pixels_+i));
  }
}

void NeoPixel::Group::off() {
  for (auto &pixel: pixels)
    pixel->off();
}

void NeoPixel::Group::set_color(uint8_t red, uint8_t green, uint8_t blue) {
  for (auto &pixel: pixels)
    pixel->set_color(red, green, blue);
}

void NeoPixel::Group::set_brightness(uint8_t brightness) {
  for (auto &pixel: pixels)
    pixel->set_brightness(brightness);
}

std::vector<NeoPixel::Pixel*> NeoPixel::Group::get_pixels() {
  return pixels;
}

NeoPixel::Controller::Controller(NeoPixel::Controller::timer_t timer_, NeoPixel::Controller::type_e type_,
                                 std::initializer_list<Group *> groups_)
 : timer(timer_),
 type(type_){

  for (auto &group: groups_) {
    for (const auto &pixel: group->get_pixels()) {
      pixels.push_back(pixel);
    }
  }
}

void NeoPixel::Controller::off() {
  for (auto &pixel: pixels)
    pixel->off();
}

void NeoPixel::Controller::set_color(uint8_t red, uint8_t green, uint8_t blue) {
  for (auto &pixel: pixels)
    pixel->set_color(red, green, blue);
}

void NeoPixel::Controller::set_brightness(uint8_t brightness) {
  for (auto &pixel: pixels)
    pixel->set_brightness(brightness);
}

void NeoPixel::Controller::update() {
  while (dma_active);
  dma_buffer.clear();
  uint32_t color;
  for (auto &pixel: pixels) {
    color = (((pixel->getColor().green * pixel->getColor().brightness / 100) << 16)
            | ((pixel->getColor().red * pixel->getColor().brightness / 100) << 8)
            | (pixel->getColor().blue * pixel->getColor().brightness / 100));
    for (int i = 23; i >=0 ; i--)
      dma_buffer.push_back((color&(1<<i)) ? LOGIC_ONE : LOGIC_ZERO);
  }
  dma_buffer.push_back(0);
  HAL_TIM_PWM_Start_DMA(timer.htim, timer.channel, dma_buffer.data(), dma_buffer.size());
  dma_active = true;
}

void NeoPixel::Controller::dma_finished() {
  HAL_TIM_PWM_Stop_DMA(timer.htim, TIM_CHANNEL_1);
  dma_active = false;
}
