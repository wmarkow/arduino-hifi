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
#include "SegmentPreAmp.h"
#include "hardware/PT2314PreAmp.h"

#define VOLUME_ANALOG_INPUT A1
#define INPUT_CHANNEL_ANALOG_INPUT A2

PT2314 pt2314;
PT2314PreAmp pt2314PreAmp(&pt2314);
SegmentPreAmp segmentPreAmp(&pt2314PreAmp);

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
   pt2314.gain(1);
}

void SegmentPreAmp::loop()
{
   checkVolumePot();
   checkInputChannelPot();
}

void SegmentPreAmp::checkVolumePot()
{
   uint16_t volumeInput = analogRead(VOLUME_ANALOG_INPUT);
   uint8_t volume = map(volumeInput, 0, 1023, preAmp->MIN_VOLUME,
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

