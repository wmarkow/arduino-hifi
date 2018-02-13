/*
 * PreAmpControlPanel.cpp
 *
 *  Created on: 29.12.2017
 *      Author: wmarkowski
 */

#include <Arduino.h>
#include <pins_arduino.h>
#include <stdint.h>
#include "SegmentPreAmp.h"

#define VOLUME_ANALOG_INPUT A1
#define INPUT_CHANNEL_ANALOG_INPUT A2

SegmentPreAmp::SegmentPreAmp(PreAmp *preAmp)
{
   this->preAmp = preAmp;
}

PreAmp* SegmentPreAmp::getPreAmp()
{
   return preAmp;
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

