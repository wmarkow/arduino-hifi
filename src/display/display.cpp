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

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
BigCrystal bigLcd(&lcd);

extern SegmentPreAmp segmentPreAmp;
extern RDA5807M radio;

void initLcd()
{
   bigLcd.begin(20, 4);
   bigLcd.setAppendExtraSpaceBetweenCharacters(false);
   lcd.home();
   lcd.clear();
   lcd.backlight();
   lcd.setBacklightPin(3, POSITIVE);
   lcd.setBacklight(HIGH);
   lcd.print("Uruchamianie...");
}

void updateDisplay(boolean checkLcdFailure)
{
   lcd.setCursor(18, 2);

   uint8_t volume = segmentPreAmp.getPreAmp()->getVolume();
   char vol[3];
   itoa(volume, vol, 10);
   if (volume <= 9)
   {
      bigLcd.setCursor(14, 2);
      bigLcd.print(F("   "));
      bigLcd.setCursor(14, 3);
      bigLcd.print(F("   "));
      bigLcd.printBig(vol, 17, 2);
   }
   else
   {
      bigLcd.printBig(vol, 14, 2);
   }

   lcd.setCursor(6, 0);

   char freq[11];
   radio.getFrequency(); // need to call it to get the current frequency from the chip
   radio.formatFrequency(freq, 11);
   bigLcd.print(freq);

   if (checkLcdFailure)
   {
      char data = (char)lcd.getCharAt(0, 3);
//      Serial.println((uint8_t)data);
      if(data != 'R' && data != 'G')
      {
         Serial.println("Probably LCD failure");
         initLcd();
      }
   }

   // display the input channel
   bigLcd.setCursor(0, 3);
   uint8_t channel = segmentPreAmp.getPreAmp()->getInputChannel();
   if (channel == 0)
   {
      bigLcd.print(F("Radio   "));
   }
   if (channel == 1)
   {
      bigLcd.print(F("Gramofon"));
   }
}
