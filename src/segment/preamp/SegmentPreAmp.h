/*
 * PreAmpControlPanel.h
 *
 *  Created on: 29.12.2017
 *      Author: wmarkowski
 */

#ifndef HARDWARE_SEGMENTPREAMP_H_
#define HARDWARE_SEGMENTPREAMP_H_

#include "hardware/PreAmp.h"

class SegmentPreAmp
{
private:
   PreAmp *preAmp;
   void checkVolumePot();
   void checkInputChannelPot();
public:
   SegmentPreAmp(PreAmp *preAmp);
   PreAmp* getPreAmp();
   void loop();
};

#endif /* HARDWARE_SEGMENTPREAMP_H_ */