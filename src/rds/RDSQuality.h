/*
 * RDSQuality.h
 *
 *  Created on: 06.02.2018
 *      Author: wmarkowski
 */

#ifndef RDSQUALITY_H_
#define RDSQUALITY_H_

class RDSQuality
{
public:
   RDSQuality();
   void loop();
   void hitValid();
   void hitInvalid();
   uint8_t getRDSQuality();
private:
   uint16_t currentValidCount;
   uint16_t currentTotalCount;
   unsigned long windowStartTimeInMillis;
};

#endif /* RDSQUALITY_H_ */
