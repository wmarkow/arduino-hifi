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
#include "../../hardware/AnalogMonostableSwitch.h"

extern LiquidCrystal_I2C lcd;

RDA5807M radio;
RDSQuality rdsQuality;
SerialRadio serialRadio(&radio);

AnalogMonostableSwitch lcdKeypadLeft(0, 0, 50);
AnalogMonostableSwitch lcdKeypadRight(0, 475, 525);

SegmentTuner segmentTuner;

SegmentTuner::SegmentTuner()
{

}

void SegmentTuner::init()
{
   lcdKeypadRight.init();
   lcdKeypadRight.setOnSwitchOnPtr(&onLcdKeypadRightPressed);
   lcdKeypadLeft.init();
   lcdKeypadLeft.setOnSwitchOnPtr(&onLcdKeypadLeftPressed);
}

void SegmentTuner::loop()
{
   lcdKeypadRight.loop();
   lcdKeypadLeft.loop();
}

void SegmentTuner::updateDisplay()
{
   lcd.setCursor(6, 0);

   char freq[11];
   radio.getFrequency(); // need to call it to get the current frequency from the chip
   radio.formatFrequency(freq, 11);
   lcd.print(freq);
}

void onLcdKeypadRightPressed()
{
   Serial.println(F("RIGHT pressed"));
   radio.seekUp(true);
}

void onLcdKeypadLeftPressed()
{
   Serial.println(F("LEFT pressed"));
   radio.seekDown(true);
}

void onLcdKeypadSelectPressed()
{
   Serial.println(F("SELECT pressed"));
}
