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

  class __attribute__ ((__packed__)) Color {
  public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t brightness;
  };

  class Pixel {
  public:

    Pixel();
    void off();
    void set_color(uint8_t red, uint8_t green, uint8_t blue);
    Color getColor();
    void set_brightness(uint8_t brightness);

  private:
    Color color;
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

    Controller(timer_t timer_, type_e type_, uint8_t number_of_pixels);
    NeoPixel::Pixel* get_pixel(uint8_t position);
    void off();
    void set_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_brightness(uint8_t brightness);
    void update();
    void dma_finished();
  private:
    timer_t timer;
    type_e type;
    std::vector<NeoPixel::Pixel*> pixels;
    bool dma_active{false};
    std::vector<uint32_t> dma_buffer;
  };

  class Group {
  public:
    Group(NeoPixel::Controller* controller, uint8_t start, uint8_t end);
    Group(NeoPixel::Controller* controller, std::initializer_list<uint8_t> indexes);
    void off();
    void set_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_brightness(uint8_t brightness);

  private:
    std::vector<Pixel*> pixels;
  };
}

#endif //STM32_NEOPIXEL_NEOPIXEL_H
