#include "Arduino.h"

#include "segment/preamp/SegmentPreAmp.h"
#include "segment/display/SegmentDisplay.h"
#include "segment/tuner/SegmentTuner.h"

unsigned long lastDisplayUpdateTime = 0;

extern SegmentPreAmp segmentPreAmp;
extern SegmentDisplay segmentDisplay;
extern SegmentTuner segmentTuner;

void setup()
{
   Serial.begin(57600);

   segmentDisplay.init();
   segmentPreAmp.init();
   segmentTuner.init();
}

void loop()
{
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
}
