/*
 * RDSQuality.cpp
 *
 *  Created on: 06.02.2018
 *      Author: wmarkowski
 */

#include <Arduino.h>
#include "RDSQuality.h"

#define WINDOW_SPAN_IN_MILLIS 2000

RDSQuality::RDSQuality()
{
   currentValidCount = 0;
   currentTotalCount = 0;
}

void RDSQuality::loop()
{
   unsigned long currentMillis = millis();
   if (currentMillis - windowStartTimeInMillis >= WINDOW_SPAN_IN_MILLIS)
   {
      currentValidCount = 0;
      currentTotalCount = 0;

      windowStartTimeInMillis = currentMillis;
   }
}

void RDSQuality::hitValid()
{
   currentValidCount++;
   currentTotalCount++;
}

void RDSQuality::hitInvalid()
{
   currentTotalCount++;
}

uint8_t RDSQuality::getRDSQuality()
{
   if (currentTotalCount == 0)
   {
      return 0;
   }

   uint16_t result = 100 * currentValidCount / currentTotalCount;

   return result;
}
