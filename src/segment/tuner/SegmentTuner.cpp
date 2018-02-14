/*
 * SegmentTuner.cpp
 *
 *  Created on: 13.02.2018
 *      Author: wmarkowski
 */

#include <LiquidCrystal_I2C.h>
#include <RDA5807M.h>

#include "SegmentTuner.h"
#include "rds/RDSQuality.h"
#include "serialradio/SerialRadio.h"

extern LiquidCrystal_I2C lcd;

RDA5807M radio;
RDSQuality rdsQuality;
SerialRadio serialRadio(&radio);

SegmentTuner::SegmentTuner()
{

}

void SegmentTuner::updateDisplay()
{
   lcd.setCursor(6, 0);

   char freq[11];
   radio.getFrequency(); // need to call it to get the current frequency from the chip
   radio.formatFrequency(freq, 11);
   lcd.print(freq);
}
