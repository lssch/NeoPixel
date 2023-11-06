//
// Created by Lars Schwarz on 13.10.2023.
//

#ifndef STM32_NEOPIXEL_NEOPIXEL_H
#define STM32_NEOPIXEL_NEOPIXEL_H

#include "stm32f4xx_hal.h"
#include <vector>
#include <array>

namespace NeoPixel{
  #define LOGIC_ZERO 25
  #define LOGIC_ONE 51

  class Pixel {
  public:
    typedef struct _color_t {
      uint8_t red;
      uint8_t green;
      uint8_t blue;
      uint8_t brightness;
    } color_t;

    Pixel();
    void off();
    void set_color(uint8_t red, uint8_t green, uint8_t blue);
    color_t getColor();
    void set_brightness(uint8_t brightness);

  private:
    color_t color;
  };

  class Group {
  public:
    Group(std::initializer_list<Pixel*> pixels_);
    Group(Pixel* pixels, uint8_t number_of_pixels);
    void off();
    void set_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_brightness(uint8_t brightness);
    std::vector<Pixel*> get_pixels();

  private:
    std::vector<Pixel*> pixels;
  };

  class Controller {
  public:
    typedef enum _type_e {
      WS2812 = 0,
      SK6812
    } type_e;

    typedef struct _timer_t {
      TIM_HandleTypeDef *htim;
      uint8_t channel;
    } timer_t;

    Controller(timer_t timer_, type_e type_, std::initializer_list<Group *> pixels_);

    void off();
    void set_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_brightness(uint8_t brightness);
    void update();
    void dma_finished();
  private:
    timer_t timer;
    type_e type;
    std::vector<Pixel*> pixels;
    bool dma_active{false};
    std::vector<uint32_t> dma_buffer;
  };
}

#endif //STM32_NEOPIXEL_NEOPIXEL_H
