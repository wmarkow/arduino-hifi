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

#define TUNER_KEYPAD_ANALOG_INPUT A0

extern LiquidCrystal_I2C lcd;

RDA5807M radio;
RDSQuality rdsQuality;
SerialRadio serialRadio(&radio);

AnalogMonostableSwitch lcdKeypadLeft(TUNER_KEYPAD_ANALOG_INPUT, 0, 150);
AnalogMonostableSwitch lcdKeypadRight(TUNER_KEYPAD_ANALOG_INPUT, 500, 700);

CRDSParser rds;

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

   radio.debugEnable();
   radio.init();
   radio.setMono(false);
   radio.setMute(false);
   radio.setVolume(15);

   radio.setFrequency(9300);
   serialRadio.init();

   rds.init();
   rds.attachServicenNameCallback(DisplayServiceName);
   rds.attachTextCallback(DisplayText);
   rds.attachTimeCallback(DisplayTime);

   radio.attachReceiveRDS(RDS_process);
}

void SegmentTuner::loop()
{
   lcdKeypadRight.loop();
   lcdKeypadLeft.loop();

   radio.checkRDS();
   rdsQuality.loop();

   serialRadio.loop();
}

void SegmentTuner::updateDisplay()
{
   char freq[11];
   radio.getFrequency(); // need to call it to get the current frequency from the chip
   radio.formatFrequency(freq, 11);

   lcd.setCursor(0, 0);
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

void RDS_process(uint16_t block1, uint16_t block2, uint16_t block3,
      uint16_t block4)
{
   rds.processData(block1, block2, block3, block4);
}

void DisplayServiceName(char *name)
{
   Serial.print("DSN:");
   Serial.println(name);

   lcd.setCursor(12, 0);
   lcd.write(name);
}

void DisplayText(char *name)
{
   Serial.print("Text:");
   Serial.println(name);
}

void DisplayTime(uint8_t hour, uint8_t minute)
{
   Serial.print("Time:");
   Serial.print(hour);
   Serial.print(":");
   Serial.println(minute);
}
