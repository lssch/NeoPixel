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

  class Color {
  public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t brightness;
  };

  class Pixel {
  public:
    /// @brief NeoPixel object which represents a single pixel.
    Pixel();

    /// @brief Switch of the pixel.
    void off();

    /// @brief Set the color of the pixel in the RGB space.
    /// @param red Red part of the color.
    /// @param green Green part of the color.
    /// @param blue Blue part of the color.
    void set_color(uint8_t red, uint8_t green, uint8_t blue);

    /// @brief Get the color of the pixel in the RGB space.
    /// @return The actual color.
    Color getColor();

    /// @brief Set the brightness of the pixel
    /// @param[in] brightness [0,100] alpha channel of the pixel.
    void set_brightness(uint8_t brightness);

  private:
    Color _color;
  };

  class Controller {
  public:
    typedef enum _type_e {
      WS2812 = 0,
      SK6812
    } type_e;

    typedef struct {
      TIM_HandleTypeDef *htim;
      uint8_t channel;
    } timer_t;

    /// @brief Neopixel controller object.
    /// @param timer Timer and channel which is connected to the output
    /// @param type Type of the pixels which are connected.
    /// @param number_of_pixels Number of pixels which ar connected.
    Controller(timer_t timer, type_e type, uint8_t number_of_pixels);

    /// @brief Get the address of the n-th pixel.
    /// @param position n-th Position.
    /// @return Address of the pixel object in memory.
    NeoPixel::Pixel* get_pixel(uint8_t position);

    /// @brief Switch off all pixels.
    void off();

    /// @brief Set the color of all pixels in the RGB space.
    /// @param red The red part of the color.
    /// @param green The green part of the color.
    /// @param blue The blue part of the color.
    void set_color(uint8_t red, uint8_t green, uint8_t blue);

    /// @brief Set the brightness of all pixels.
    /// @param brightness [0,100] alpha channel of the pixels.
    void set_brightness(uint8_t brightness);

    /// @brief Update all pixels.
    /// @attention The connected only updates the leds when this function is called.
    /// @todo Better way to update the controller or maybe detect when a pixel is changed. If implemented correctly this function can be eliminated.
    void update();

    /// @brief Feedback when the dma transaction has finished.
    void dma_finished();
  private:
    timer_t _timer;
    type_e _type;
    std::vector<NeoPixel::Pixel*> _pixels;
    bool _dma_active;
    std::vector<uint32_t> _dma_buffer;
  };

  class Group {
  public:
    /// @brief Neopixel group object to group leds in software together.
    /// @param controller The controller which the leds are connected to.
    /// @param start The n-th led of the controller to start the group.
    /// @param end The n-th led of the controller to end the group.
    Group(NeoPixel::Controller* controller, uint8_t start, uint8_t end);

    /// @brief Neopixel group object to group leds in software together.
    /// @param controller The controller which the leds are connected to.
    /// @param indexes List of the pixels to group.
    Group(NeoPixel::Controller* controller, std::initializer_list<uint8_t> indexes);

    /// @brief Switch off all pixels.
    void off();

    /// @brief Set the color of all pixels in the RGB space.
    /// @param red The red part of the color.
    /// @param green The green part of the color.
    /// @param blue The blue part of the color.
    void set_color(uint8_t red, uint8_t green, uint8_t blue);

    /// @brief Set the brightness of all pixels.
    /// @param brightness [0,100] alpha channel of the pixels.
    void set_brightness(uint8_t brightness);

  private:
    std::vector<Pixel*> _pixels;
  };
}

#endif //STM32_NEOPIXEL_NEOPIXEL_H
