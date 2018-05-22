/*
 * gfx.cpp
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

#include "gfx.h"

using namespace gfx;

#define CLIPX(x, w) \
  if (x + w > WIDTH) w = WIDTH - x; \
  if (x < 0) { w += x; x = 0; } \
  if (w <= 0) return; \
  do {} while (0)

#define CLIPY(y, h) \
  if (y + h > HEIGHT) h = HEIGHT - y; \
  if (y < 0) { h += y; y = 0; } \
  if (h <= 0) return; \
  do {} while (0)

GFX::GFX(coord_t w, coord_t h)
: WIDTH(w)
, HEIGHT(h)
, rotation(0)
{

}


void GFX::drawHLine(coord_t x, coord_t y, coord_t w)
{
  coord_t h = 1;
  CLIPX(x, w);
  CLIPY(y, h);
  uint8_t *start = get_frame_ptr(x, y);

  //draw_pixel_row<DRAW_NORMAL>(start, w, 0x1 << (y & 0x7));
}
