/*
 * SegmentDisplay.cpp
 *
 *  Created on: 13.02.2018
 *      Author: wmarkowski
 */

#include <LiquidCrystal_I2C.h>
#include <BigCrystal.h>

#include "SegmentDisplay.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
BigCrystal bigLcd(&lcd);

SegmentDisplay segmentDisplay;

SegmentDisplay::SegmentDisplay()
{

}

void SegmentDisplay::init()
{
   bigLcd.begin(20, 4);
   bigLcd.setAppendExtraSpaceBetweenCharacters(false);
   lcd.home();
   lcd.backlight();
   lcd.setBacklightPin(3, POSITIVE);
   lcd.setBacklight(HIGH);
   lcd.clear();
}

void SegmentDisplay::checkPhonoIssue()
{
   char data = (char) lcd.getCharAt(0, 3);
   //      Serial.println((uint8_t)data);
   if (data != 'R' && data != 'G')
   {
      Serial.println("Probably LCD failure");
      init();
   }
}
