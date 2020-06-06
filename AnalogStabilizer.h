/*
 * AnalogStabilizer.h
 *
 *  Created on: 06.06.2020
 *      Author: cybaer
 */

#ifndef ANALOGSTABILIZER_H_
#define ANALOGSTABILIZER_H_

#include <stdint.h>

static const uint8_t STAB_BITS = 1;
static const uint8_t STAB = (1 << STAB_BITS);

class AnalogStabilizer
{
public:
  AnalogStabilizer()
  : m_Count(0)
  {}

  uint16_t operator() (uint16_t in)
  {
    m_Raw[m_Count++ & (STAB - 1)] = in;
    uint16_t sum = 0;
    for(int8_t i=0; i<STAB; i++)
    {
      sum += m_Raw[i];
    }
    return sum / STAB;
  }
private:
  uint16_t m_Raw[STAB];
  uint8_t m_Count;
};


#endif /* ANALOGSTABILIZER_H_ */
