#include "Arduino.h"

#include <Wire.h>
#include <CrashTracking/ApplicationMonitor.h>

#include "segment/preamp/SegmentPreAmp.h"
#include "segment/display/SegmentDisplay.h"
#include "segment/tuner/SegmentTuner.h"

Watchdog::CApplicationMonitor ApplicationMonitor;

unsigned long lastDisplayUpdateTime = 0;

extern SegmentPreAmp segmentPreAmp;
extern SegmentDisplay segmentDisplay;
extern SegmentTuner segmentTuner;

void setup()
{
   Serial.begin(57600);
   Wire.setTimeoutInMillis(50);

   ApplicationMonitor.Dump(Serial);
   ApplicationMonitor.EnableWatchdog(Watchdog::CApplicationMonitor::Timeout_4s);

   segmentDisplay.init();
   segmentPreAmp.init();
   segmentTuner.init();

   pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
   ApplicationMonitor.IAmAlive();

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

      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
   }
}
