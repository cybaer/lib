/*
 * multivider.h
 *
 *  Created on: 04.03.2017
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

#ifndef MULTIVIDER_H_
#define MULTIVIDER_H_

#include <inttypes.h>

class Multiplier
{
public:
  Multiplier(void)
  : m_Tick(0)
  , m_LastPeriodeTick(0)
  , m_PeriodeTicks(0)
  , m_OldValue(false)
  , m_Factor(1)
  , m_AdditionalPulsesCount(0)
  , m_NextPulseStartTick(0)
  , m_PulseEndTick(0)
  , m_InPulseStart(0)
  , m_PulseLen(0)
  , m_Pulse(false)
  { }

  bool operator() (bool in)
  {
    calculatePulseLen(in);
    if(isRisingEdge(in))
    {
      calculateNewPeriod();
      outStartPulse();
    }
    outAdditionalPulse();
    endPulse();
    m_OldValue = in;
    return m_Pulse;
  }
  void onTick(void) { m_Tick++; }
  void setFactor(uint8_t factor) { m_Factor = factor; }

private:
  inline void calculatePulseLen(bool const in)
  {
    if(isRisingEdge(in))
    {
      m_InPulseStart = m_Tick;
    }
    if(isFallingEdge(in))
    {
      m_PulseLen = m_Tick - m_InPulseStart;
    }
  }
  inline void calculateNewPeriod(void) // must be called at the same point of every periode (e.g. rising edge)
  {
    const uint16_t inputPeriodeTicks = m_Tick - m_LastPeriodeTick;
    const uint8_t roundingOffset = m_Factor / 2;
    m_PeriodeTicks = (inputPeriodeTicks + roundingOffset) / m_Factor;
    m_LastPeriodeTick = m_Tick;
  }
  inline void outStartPulse(void)
  {
    m_AdditionalPulsesCount = m_Factor - 1;
    newPulse(m_Tick + m_PeriodeTicks);
    m_Pulse = true;
  }
  inline void outAdditionalPulse(void)
  {
    if(m_AdditionalPulsesCount > 0 && int16_t(m_NextPulseStartTick - m_Tick) <= 0)
    {
      m_AdditionalPulsesCount--;
      newPulse(m_NextPulseStartTick += m_PeriodeTicks);
      m_Pulse = true;
    }
  }
  inline void endPulse(void)
  {
    if(m_Pulse && int16_t(m_PulseEndTick - m_Tick) <= 0)
    {
      m_Pulse = false;
    }
  }
  inline bool isRisingEdge(bool const in) const { return in && !m_OldValue; };
  inline bool isFallingEdge(bool const in) const { return !in && m_OldValue; };
  inline void newPulse(uint16_t startTick)
  {
    m_NextPulseStartTick = startTick;
    m_PulseEndTick = m_Tick + m_PulseLen;
  }
  uint16_t m_Tick;
  uint16_t m_LastPeriodeTick;
  uint16_t m_PeriodeTicks;
  bool m_OldValue;
  uint8_t m_Factor;
  uint8_t m_AdditionalPulsesCount;
  uint16_t m_NextPulseStartTick;
  uint16_t m_PulseEndTick;
  uint16_t m_InPulseStart;
  uint16_t m_PulseLen;
  bool m_Pulse;
};


class Divider
{
public:
  Divider(void)
  : m_Tick(0)
  , m_Divisor(1)
  , m_OldValue(false)
  , m_Action(false)
  , m_Count(0)
  {}

  bool operator()(bool in)
  {
    if(in)
    {
      if(in != m_OldValue)
      {
        m_OldValue = in;
        if(++m_Count >= m_Divisor)
        {
          m_Count = 0;
          m_Action = true;
        }
      }
    }
    else
    { // reproducing the original pulse length
      m_Action = false;
      m_OldValue = false;
    }
    return m_Action;
  }
  void onTick(void) { m_Tick++; }
  void setDivisor(uint8_t divisor) { m_Divisor = divisor; }

private:

  uint16_t m_Tick;
  uint8_t m_Divisor;
  bool m_OldValue;
  bool m_Action;
  uint8_t m_Count;
};

class Multivider
{
public:
  bool operator()(bool in)
  {
    return m_Divider(m_Multiplier(in));
  }
  void onTick(void) {m_Multiplier.onTick(); m_Divider.onTick(); };
  void setFactor(uint8_t factor) { m_Multiplier.setFactor(factor); };
  void setDivisor(uint8_t divisor) { m_Divider.setDivisor(divisor); };
private:
  Multiplier m_Multiplier;
  Divider m_Divider;
};

#endif /* MULTIVIDER_H_ */
