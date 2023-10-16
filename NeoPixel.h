//
// Created by Lars Schwarz on 13.10.2023.
//

#ifndef STM32_NEOPIXEL_NEOPIXEL_H
#define STM32_NEOPIXEL_NEOPIXEL_H

#include "stm32f4xx_hal.h"
#include <vector>

namespace NeoPixel{
  #define LOGIC_ZERO 25
  #define LOGIC_ONE 51

  class Pixel {
  public:
    Pixel();
    void off();
    void set_color(uint8_t, uint8_t, uint8_t);
    void set_brightness(uint8_t);
    uint8_t red{};
    uint8_t green{};
    uint8_t blue{};
    uint8_t brightness{};
  private:
  };

  class Controller {
  public:
    typedef enum {
      WS2812 = 0,
      SK6812
    } type_e;

    Controller(TIM_HandleTypeDef *htim_, uint8_t channel_, type_e type_, uint8_t number_of_pixels);
    void add_pixel(uint8_t number_of_pixels);
    void off();
    void set_color(uint8_t, uint8_t, uint8_t);
    void set_brightness(uint8_t);
    void update();
    void dma_finished();
    std::vector<Pixel> pixels;
  private:
    TIM_HandleTypeDef *htim;
    uint8_t channel;
    type_e type;
    bool dma_active{false};
    std::vector<uint32_t> dma_buffer;
  };

}

#endif //STM32_NEOPIXEL_NEOPIXEL_H
