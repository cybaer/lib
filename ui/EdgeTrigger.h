/*
 * EdgeTrigger.h
 *
 *  Created on: 06.03.2018
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

#ifndef EDGETRIGGER_H_
#define EDGETRIGGER_H_

#include "avrlib/gpio.h"
using namespace avrlib;

template<typename Input, uint8_t ActiveLevel=0>
class EdgeTrigger
{
public:
  static inline void init(void)
  {
    Input::set_mode(DIGITAL_INPUT);
    if(ActiveLevel == 0) Input::High();
  }
  static inline bool isTriggered(void)
  {
    uint8_t in = Input::value();
    bool ret = in != OldValue && in == ActiveLevel;
    OldValue = in;
    return ret;
  }
  static bool getValue(void) { return Input::value(); };
private:
  static uint8_t OldValue;
};

template<typename Input, uint8_t ActiveLevel>
uint8_t EdgeTrigger<Input, ActiveLevel>::OldValue;







#endif /* EDGETRIGGER_H_ */
