/*
 * display.cpp
 *
 *  Created on: 13.02.2018
 *      Author: wmarkowski
 */

#include <LiquidCrystal_I2C.h>
#include <BigCrystal.h>
#include <RDA5807M.h>
#include "display.h"

#include "../segment/preamp/SegmentPreAmp.h"

extern SegmentPreAmp segmentPreAmp;
extern RDA5807M radio;

extern LiquidCrystal_I2C lcd;
extern BigCrystal bigLcd;

void updateDisplay(boolean checkLcdFailure)
{
   lcd.setCursor(6, 0);

   char freq[11];
   radio.getFrequency(); // need to call it to get the current frequency from the chip
   radio.formatFrequency(freq, 11);
   bigLcd.print(freq);
}
