/*
 * SegmentTuner.h
 *
 *  Created on: 13.02.2018
 *      Author: wmarkowski
 */

#ifndef SEGMENT_TUNER_SEGMENTTUNER_H_
#define SEGMENT_TUNER_SEGMENTTUNER_H_

class SegmentTuner
{
public:
   SegmentTuner();
   void init();
   void loop();
   void updateDisplay();
private:
};

void onLcdKeypadRightPressed();
void onLcdKeypadLeftPressed();
void onLcdKeypadSelectPressed();

void RDS_process(uint16_t block1, uint16_t block2, uint16_t block3,
      uint16_t block4);
void DisplayServiceName(char *name);
void DisplayText(char *name);
void DisplayTime(uint8_t hour, uint8_t minute);

#endif /* SEGMENT_TUNER_SEGMENTTUNER_H_ */
