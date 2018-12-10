/*
 * gfx.h
 *
 *  Created on: 19.06.2016
 *      Author: cybaer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GFX_H_
#define GFX_H_

#include "avrlib/base.h"

namespace gfx
{

  typedef int_fast8_t coord_t;

  class GFX
  {
   public:
    GFX(coord_t w, coord_t h); // Constructor

    // MUST be overwritten by subclass:
    virtual void drawPixel(coord_t x, coord_t y, uint8_t color) = 0;

    // MAY be overridden by the subclass
    virtual void drawLine(coord_t x0, coord_t y0, coord_t x1, coord_t y1, uint8_t color);

    void drawHLine(coord_t x, coord_t y, coord_t w);
    virtual void drawRect(coord_t x, coord_t y, coord_t w, coord_t h, uint8_t color);
    virtual void fillRect(coord_t x, coord_t y, coord_t w, coord_t h, uint8_t color);

    void drawChar(coord_t x, coord_t y, unsigned char c, uint8_t color, uint16_t bg, uint8_t size);
    void setCursor(coord_t x, coord_t y);
    void setTextColor(uint8_t c);
    void setTextColor(uint8_t c, uint8_t bg);
    void setTextSize(uint8_t s);
    void setTextWrap(boolean w);
    void setRotation(uint8_t r);

    virtual void write(uint8_t);

    coord_t height(void) const { return HEIGHT; }
    coord_t width(void) const { return WIDTH; }

    uint8_t getRotation(void) const{ return rotation; }

   protected:
     const coord_t WIDTH;
     const coord_t HEIGHT;
     uint8_t rotation;

   private:
     uint8_t *frame_;
     inline uint8_t *get_frame_ptr(const coord_t x, const coord_t y) __attribute__((always_inline));
  };

  inline uint8_t *GFX::get_frame_ptr(const coord_t x, const coord_t  y) { return frame_ + ((y >>3) << 7) + x; }

}

#endif /* GFX_H_ */
