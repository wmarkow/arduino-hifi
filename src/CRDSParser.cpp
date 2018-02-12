///
/// \file RDSParser.cpp
/// \brief RDS Parser class implementation.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2014 by Matthias Hertel.\n
/// This work is licensed under a BSD style license.\n
/// See http://www.mathertel.de/License.aspx
///
/// \details
///
/// More documentation and source code is available at http://www.mathertel.de/Arduino
///
/// ChangeLog see RDSParser.h.

#include "CRDSParser.h"
#include "RDSQuality.h"

#define DEBUG_FUNC0(fn)          { Serial.print(fn); Serial.println("()"); }

extern RDSQuality rdsQuality;

/// Setup the RDS object and initialize private variables to 0.
CRDSParser::CRDSParser()
{
   memset(this, 0, sizeof(CRDSParser));
} // CRDSParser()

void CRDSParser::init()
{
   strcpy(currentPSName, "--------");
   memset(_RDSText, 0, sizeof(_RDSText));
   _lastTextIDX = 0;
} // init()

void CRDSParser::attachServicenNameCallback(
      receiveServicenNameFunction newFunction)
{
   _sendServiceName = newFunction;
} // attachServicenNameCallback

void CRDSParser::attachTextCallback(receiveTextFunction newFunction)
{
   _sendText = newFunction;
} // attachTextCallback

void CRDSParser::attachTimeCallback(receiveTimeFunction newFunction)
{
   _sendTime = newFunction;
} // attachTimeCallback

void CRDSParser::processData(uint16_t block1, uint16_t block2, uint16_t block3,
      uint16_t block4)
{
   // DEBUG_FUNC0("process");
   uint8_t idx; // index of rdsText
   char c1, c2;
   char *p;

   uint16_t mins; ///< RDS time in minutes
   uint8_t off;   ///< RDS time offset and sign

   if (!isValid(block1, block2, block3))
   {
      rdsQuality.hitInvalid();
      return;
   }

//   Serial.print('(');
//   Serial.print(block1, HEX);
//   Serial.print(' ');
//   Serial.print(block2, HEX);
//   Serial.print(' ');
//   Serial.print(block3, HEX);
//   Serial.print(' ');
//   Serial.println(block4, HEX);

   if (block1 == 0)
   {
      // reset all the RDS info.
      init();
      rdsQuality.hitInvalid();

      // Send out empty data
      if (_sendServiceName)
         _sendServiceName(currentPSName);
      if (_sendText)
         _sendText("");

      return;
   }

   rdsQuality.hitValid();

   // analyzing Block 2
   rdsGroupType = 0x0A | ((block2 & 0xF000) >> 8) | ((block2 & 0x0800) >> 11);
   rdsTP = (block2 & 0x0400);
   rdsPTY = (block2 & 0x0400);

   switch (rdsGroupType)
   {
      case 0x0A:
      case 0x0B:
         // The data received is part of the Service Station Name
         idx = 2 * (block2 & 0x0003);

         // new data is 2 chars from block 4
         c1 = block4 >> 8;
         c2 = block4 & 0x00FF;

         currentPSName[idx] = c1;
         currentPSName[idx + 1] = c2;
         currentPSName[8] = '\0';

         if (idx == 6)
         {
            // received last two characters; publish station name
            if (_sendServiceName)
            {
               _sendServiceName(currentPSName);
            }
         }

         break;

      case 0x2A:
         // The data received is part of the RDS Text.
         _textAB = (block2 & 0x0010);
         idx = 4 * (block2 & 0x000F);

         if (idx < _lastTextIDX)
         {
            // the existing text might be complete because the index is starting at the beginning again.
            // now send it to the possible listener.
            if (_sendText)
               _sendText(_RDSText);
         }
         _lastTextIDX = idx;

         if (_textAB != _last_textAB)
         {
            // when this bit is toggled the whole buffer should be cleared.
            _last_textAB = _textAB;
            memset(_RDSText, 0, sizeof(_RDSText));
            // Serial.println("T>CLEAR");
         } // if

         // new data is 2 chars from block 3
         _RDSText[idx] = (block3 >> 8);
         idx++;
         _RDSText[idx] = (block3 & 0x00FF);
         idx++;

         // new data is 2 chars from block 4
         _RDSText[idx] = (block4 >> 8);
         idx++;
         _RDSText[idx] = (block4 & 0x00FF);
         idx++;

         // Serial.print(' '); Serial.println(_RDSText);
         // Serial.print("T>"); Serial.println(_RDSText);
         break;

      case 0x4A:
         // Clock time and date
         off = (block4) & 0x3F; // 6 bits
         mins = (block4 >> 6) & 0x3F; // 6 bits
         mins += 60 * (((block3 & 0x0001) << 4) | ((block4 >> 12) & 0x0F));

         // adjust offset
         if (off & 0x20)
         {
            mins -= 30 * (off & 0x1F);
         }
         else
         {
            mins += 30 * (off & 0x1F);
         }

         if ((_sendTime) && (mins != _lastRDSMinutes))
         {
            _lastRDSMinutes = mins;
            _sendTime(mins / 60, mins % 60);
         } // if
         break;

      case 0x6A:
         // IH
         break;

      case 0x8A:
         // TMC
         break;

      case 0xAA:
         // TMC
         break;

      case 0xCA:
         // TMC
         break;

      case 0xEA:
         // IH
         break;

      default:
         // Serial.print("RDS_GRP:"); Serial.println(rdsGroupType, HEX);
         break;
   }
} // processData()

bool CRDSParser::isValid(uint16_t block1, uint16_t block2, uint16_t block3)
{
   uint8_t block1CountryCode = block1 >> 12;

   // validate groups from Poland
   if (block1CountryCode != 3)
   {
      return false;
   }

   uint8_t versionCode = ((block2 & 0x0800) >> 11);
   if (versionCode == 1 && (block1 != block3))
   {
      // PI code mismatch in version B
      return false;
   }

   return true;
}

// End.
