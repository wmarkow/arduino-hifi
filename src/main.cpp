#include <Wire.h>
#include <RDSParser.h>
#include <LiquidCrystal_I2C.h>
#include <RDA5807M.h>

#include "Arduino.h"

#include "segment/tuner/serialradio/SerialRadio.h"
#include "segment/tuner/rds/RDSQuality.h"
#include "segment/preamp/SegmentPreAmp.h"
#include "segment/display/SegmentDisplay.h"
#include "segment/tuner/SegmentTuner.h"

unsigned long lastDisplayUpdateTime = 0;

extern LiquidCrystal_I2C lcd;
extern SegmentPreAmp segmentPreAmp;
extern SegmentDisplay segmentDisplay;
extern SegmentTuner segmentTuner;

extern RDA5807M radio;
extern SerialRadio serialRadio;
extern RDSQuality rdsQuality;

void setup()
{
   Serial.begin(57600);

   segmentDisplay.init();
   segmentTuner.init();

   Serial.begin(57600);
   Serial.print("Radio...");
   delay(500);

   segmentPreAmp.init();

   radio.debugEnable();
   radio.init();
   radio.setMono(false);
   radio.setMute(false);
   radio.setVolume(15);

   radio.setFrequency(9300);
   serialRadio.init();

   lcd.clear();
}

void loop()
{
   rdsQuality.loop();

   segmentPreAmp.loop();
   segmentTuner.loop();

   if (millis() - lastDisplayUpdateTime > 250)
   {
      segmentDisplay.checkPhonoIssue();
      segmentPreAmp.updateDisplay();
      segmentTuner.updateDisplay();

      lastDisplayUpdateTime = millis();
//      Serial.print("RDSQ= ");
//      Serial.println(rdsQuality.getRDSQuality());
   }

   radio.checkRDS();

   serialRadio.loop();
}
