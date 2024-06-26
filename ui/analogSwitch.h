/*
 * analogSwitch.h
 *
 *  Created on: 18.03.2017
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

#ifndef ANALOGSWITCH_H_
#define ANALOGSWITCH_H_

#include "avrlib/adc.h"
#include <stdint.h>

template<uint8_t Count, int8_t Hyst>
class Mapper
{
public:

  static void init(void)
  {
    for(int8_t i = 0; i<Count;i++)
    {
      Greater[i] = ((i+1)*0x00FFLU)/Count + Hyst;
      Smaller[i] = ((i)*0x00FFLU)/Count - Hyst;
    }
    Greater[Count-1] = 0xFF;
    Smaller[0] = 0;
  }
  static uint8_t getNewIndex(uint8_t const in, uint8_t const index)
  {
    uint8_t newIndex = in > Greater[index] ? index+1
                     : in < Smaller[index] ? index-1
                     : index;
   return newIndex;
  }
  static const uint8_t Section = (0xFF / Count) +1;
  static uint8_t Greater[Count];
  static uint8_t Smaller[Count];
};
template<uint8_t Count, int8_t Hyst>
uint8_t Mapper<Count, Hyst>::Greater[Count];
template<uint8_t Count, int8_t Hyst>
uint8_t Mapper<Count, Hyst>::Smaller[Count];

// partial template specialization for non hysteresis
template<uint8_t Count>
class Mapper<Count, 0>
{
public:

  static void init(void){}
  static uint8_t getNewIndex(uint8_t in, uint8_t const)
  {
    return (in & 0xFE) / Section;
  }
  static const uint8_t Section = (0xFF / Count) +0;
};

template <class Adc, uint8_t Port, uint8_t Count, int8_t Hyst = 0>
class AnalogSwitch
{
public:
  typedef Mapper<Count, Hyst> Map;
  static uint8_t getValue(void)
  {
    return Map::getNewIndex(Adc::Read(Port) >> 2, 0);
  }
  static uint8_t getNewIndex(uint8_t const index)
  {
    return Map::getNewIndex(Adc::Read(Port) >> 2, index);
  }
  static void init(void) { Map::init(); }
};

template <class Adc, uint8_t Port, int8_t R1, int8_t R2, int8_t R3, int8_t R4>
class AnalogBinSwitch
{
  static const uint16_t Vin = 255LU;
  static const uint16_t V0 = (R3 + R4) * Vin / (R1 + R2 + R3 + R4);
  static const uint16_t V1 = (R3 + R4) * Vin / (R1 + R3 + R4);
  static const uint16_t V2 = R4 * Vin / (R1 + R2 + R4);
  static const uint16_t V3 = R4 * Vin / (R1 + R4);
  static const uint8_t T2 = (V2 + V0) / 2;
  static const uint8_t T0 = (V0 + V3) / 2;
  static const uint8_t T3 = (V3 + V1) / 2;

public:
  static uint16_t getValue(void) { return Adc::Read(Port) >> 2; }
  static uint8_t getButton(void)
  {
    uint8_t val = Adc::Read(Port) >> 2;
    return    val < T2 ? 2 
            : val < T0 ? 0
            : val < T3 ? 3
            : 1 ;
  }
};
#endif /* ANALOGSWITCH_H_ */
