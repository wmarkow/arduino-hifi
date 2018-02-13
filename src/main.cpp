#include <Wire.h>
#include <RDSParser.h>
#include <LiquidCrystal_I2C.h>
#include <PT2314.h>

#include "Arduino.h"
#include "hardware/AnalogMonostableSwitch.h"
#include "segment/tuner/serialradio/SerialRadio.h"
#include "segment/preamp/hardware/PT2314PreAmp.h"
#include "segment/preamp/hardware/RDA5807PreAmp.h"
#include "segment/tuner/rds/RDSQuality.h"
#include "display/display.h"
#include "segment/preamp/SegmentPreAmp.h"

// hardware objects
RDA5807M radio;
PT2314 pt2314;
RDSQuality rdsQuality;

// preamplifiers
//RDA5807PreAmp rdaPreAmp(&radio);
PT2314PreAmp pt2314PreAmp(&pt2314);

SegmentPreAmp segmentPreAmp(&pt2314PreAmp);
//PreAmpControlPanel preAmpControlPanel(&radio);

SerialRadio serialRadio(&radio);
AnalogMonostableSwitch lcdKeypadLeft(0, 0, 50);
AnalogMonostableSwitch lcdKeypadRight(0, 475, 525);

unsigned long lastDisplayUpdateTime = 0;
unsigned long lastRdsCheckTime = 0;

extern LiquidCrystal_I2C lcd;

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

void setup()
{
   Serial.begin(57600);

   initLcd();

   lcdKeypadRight.init();
   lcdKeypadRight.setOnSwitchOnPtr(&onLcdKeypadRightPressed);
   lcdKeypadLeft.init();
   lcdKeypadLeft.setOnSwitchOnPtr(&onLcdKeypadLeftPressed);

   Serial.begin(57600);
   Serial.print("Radio...");
   delay(500);

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

   radio.debugEnable();
   radio.init();
   radio.setMono(false);
   radio.setMute(false);
   radio.setVolume(15);

   radio.setFrequency(9300);
   serialRadio.init();

   lcd.clear();
   updateDisplay(false);
}

void loop()
{
   lcdKeypadRight.loop();
   lcdKeypadLeft.loop();
   rdsQuality.loop();

   if (millis() - lastDisplayUpdateTime > 250)
   {
      updateDisplay(true);
      segmentPreAmp.loop();
      lastDisplayUpdateTime = millis();
//      Serial.print("RDSQ= ");
//      Serial.println(rdsQuality.getRDSQuality());
   }

   radio.checkRDS();

   serialRadio.loop();
}
