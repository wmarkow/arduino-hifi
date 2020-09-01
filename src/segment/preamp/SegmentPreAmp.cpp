/*
 * PreAmpControlPanel.cpp
 *
 *  Created on: 29.12.2017
 *      Author: wmarkowski
 */

#include <Arduino.h>
#include <pins_arduino.h>
#include <stdint.h>

#include <PT2314.h>
#include <LiquidCrystal_I2C.h>
#include <BigCrystal.h>
#include "SegmentPreAmp.h"
#include "hardware/PT2314PreAmp.h"

#define VOLUME_ANALOG_INPUT A1
#define INPUT_CHANNEL_ANALOG_INPUT A3

PT2314 pt2314;
PT2314PreAmp pt2314PreAmp(&pt2314);
SegmentPreAmp segmentPreAmp(&pt2314PreAmp);

extern LiquidCrystal_I2C lcd;
extern BigCrystal bigLcd;


#define VOLUME_MAX_DELTA 20
#define VOLUME_MAX 1023 / VOLUME_MAX_DELTA / 2

SegmentPreAmp::SegmentPreAmp(PreAmp *preAmp)
{
   this->preAmp = preAmp;
}

PreAmp* SegmentPreAmp::getPreAmp()
{
   return preAmp;
}

void SegmentPreAmp::init()
{
   uint8_t q = 0;
   bool res = false;
   for (q = 0; q < 20; q++)
   {
      res = pt2314.init();
      if (res == true)
      {
         break;
      }
      delay(100);
   }

   pt2314.channel(0);
   pt2314.volume(1);
   pt2314.attenuation(100, 100);
   pt2314.gain(3);
}

void SegmentPreAmp::loop()
{
   checkVolumePot();
   checkInputChannelPot();
}

void SegmentPreAmp::updateDisplay()
{
   lcd.setCursor(18, 2);

   uint8_t volume = segmentPreAmp.getPreAmp()->getVolume();
   if (volume >= 100)
   {
      // we have only two digits to show volume
      volume = 99;
   }
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

void SegmentPreAmp::checkVolumePot()
{
   // The reading from ADC of the potentiometer can have a variation (deviation).
   // The 'VOLUME_MAX_DELTA' variable is the max wide of those variations. To avoid volume
   // changes because of this variations the whole potentiometer area is divided
   // into sections.
   // So we have: volume section, void section, volume section, void section,...
   // volume section is the section where a real volume can be set
   // void section is just a space where analog reading variations can be omitted.
   // between real volume sections.
   // Max potentiometer reading is 1023. Every section is 'VOLUME_MAX_DELTA' wide.
   uint16_t volumeInput = analogRead(VOLUME_ANALOG_INPUT);
   uint8_t sectionNumber = volumeInput / VOLUME_MAX_DELTA;
   if (sectionNumber % 2 == 1)
   {
      // this is a void space of the potentiometer
      // do nothing
      return;
   }
   // we have a real volume reading
   uint8_t volumeSection = sectionNumber / 2;
   uint8_t volume = map(volumeSection, 0, VOLUME_MAX, preAmp->MIN_VOLUME,
         preAmp->MAX_VOLUME);
   preAmp->setVolume(volume);
}

void SegmentPreAmp::checkInputChannelPot()
{
   uint16_t volumeInput = analogRead(INPUT_CHANNEL_ANALOG_INPUT);

   uint8_t channel = 0;
   if (volumeInput <= 512)
   {
      channel = 0;
   }
   else if (volumeInput <= 1023)
   {
      channel = 1;
   }

   if (preAmp->getInputChannel() == channel)
   {
      return;
   }

   preAmp->setInputChannel(channel);
}

