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

#endif /* SEGMENT_TUNER_SEGMENTTUNER_H_ */
