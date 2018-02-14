#include <Wire.h>
#include <RDSParser.h>
#include <LiquidCrystal_I2C.h>
#include <RDA5807M.h>

#include "Arduino.h"
#include "hardware/AnalogMonostableSwitch.h"
#include "segment/tuner/serialradio/SerialRadio.h"
#include "segment/tuner/rds/RDSQuality.h"
#include "segment/preamp/SegmentPreAmp.h"
#include "segment/display/SegmentDisplay.h"
#include "segment/tuner/SegmentTuner.h"

AnalogMonostableSwitch lcdKeypadLeft(0, 0, 50);
AnalogMonostableSwitch lcdKeypadRight(0, 475, 525);

unsigned long lastDisplayUpdateTime = 0;

extern LiquidCrystal_I2C lcd;
extern SegmentPreAmp segmentPreAmp;
extern SegmentDisplay segmentDisplay;
extern SegmentTuner segmentTuner;

extern RDA5807M radio;
extern SerialRadio serialRadio;
extern RDSQuality rdsQuality;

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

   segmentDisplay.init();

   lcdKeypadRight.init();
   lcdKeypadRight.setOnSwitchOnPtr(&onLcdKeypadRightPressed);
   lcdKeypadLeft.init();
   lcdKeypadLeft.setOnSwitchOnPtr(&onLcdKeypadLeftPressed);

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
   lcdKeypadRight.loop();
   lcdKeypadLeft.loop();
   rdsQuality.loop();

   if (millis() - lastDisplayUpdateTime > 250)
   {
      segmentDisplay.checkPhonoIssue();
      segmentPreAmp.updateDisplay();
      segmentTuner.updateDisplay();

      segmentPreAmp.loop();
      lastDisplayUpdateTime = millis();
//      Serial.print("RDSQ= ");
//      Serial.println(rdsQuality.getRDSQuality());
   }

   radio.checkRDS();

   serialRadio.loop();
}
