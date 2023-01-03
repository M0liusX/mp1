#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <map>
typedef unsigned char byte;

struct TrackEvent // referenced in order, but first = 2710, doors refer to 27XX + preset to it implicitly, 0x44 per
{
	bool obsoleteEvent;
	unsigned long deltaTime;
	unsigned long durationTime;
	unsigned long absoluteTime;
	byte type;
	byte* contents;  // remember delete all track events mem later
	int contentSize;

	TrackEvent()
	{
		type = 0x00;

		contents = NULL;
		contentSize = 0;

		deltaTime = 0;
		absoluteTime = 0;
		obsoleteEvent = false;
		durationTime = 0;
	}
};

std::vector<byte> LoadBinary(std::string filename)
{
   std::ifstream file(filename.c_str(), std::ios::ate | std::ios::binary);
   assert(file.is_open() && "Bad filename!");
   size_t fileSize = static_cast<size_t>(file.tellg());
   
   std::vector<byte> binary(fileSize);
   file.seekg(0);
   file.read(reinterpret_cast<char*>(binary.data()), fileSize);
   file.close();

   return binary;
}

unsigned long Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}
unsigned long CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}


unsigned short Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}
unsigned short CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

byte ReadMidiByte(byte* vlByteArray, int& offset, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats)
{
	byte returnByte;
	if (altPattern != NULL)
	{
		returnByte = altPattern[altOffset];
		altOffset++;
	}
	else
	{
		returnByte = vlByteArray[offset];
		offset++;

		if ((returnByte == 0xFE) && (vlByteArray[offset] != 0xFE) && includeFERepeats)
		{
			byte repeatFirstByte = vlByteArray[offset];
			offset++;

			unsigned long repeatDistanceFromBeginningMarker = ((repeatFirstByte << 8) | vlByteArray[offset]);
			offset++;
			byte repeatCount = vlByteArray[offset];
			offset++;

			altPattern = new byte[repeatCount];
			for (int copy = ((offset - 4) - repeatDistanceFromBeginningMarker); copy < (((offset - 4) - repeatDistanceFromBeginningMarker) + repeatCount); copy++)
			{
				altPattern[copy - ((offset - 4) - repeatDistanceFromBeginningMarker)] = vlByteArray[copy];
			}
			altOffset = 0;
			altLength = repeatCount;

			returnByte = altPattern[altOffset];
			altOffset++;
		}
		else if ((returnByte == 0xFE) && (vlByteArray[offset] == 0xFE) && includeFERepeats)
		{
			// skip duplicate FEs
			offset++;
		}
	}

	if ((altOffset == altLength) && (altPattern != NULL))
	{
		delete [] altPattern;
		altPattern = NULL;
		altOffset = 0;
		altLength = 0;
	}
	
	return returnByte;
}

unsigned long GetVLBytes(byte* vlByteArray, int& offset, unsigned long& original, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats)
{
    unsigned long VLVal = 0; //Vlength Value.
    byte TempByte; //Byte value read.

    for (; ; )
    {
		if (altPattern != NULL)
		{
			TempByte = altPattern[altOffset];
			altOffset++;

			if (altOffset == altLength)
			{
				delete [] altPattern;
				altPattern = NULL;
				altOffset = 0;
				altLength = 0;
			}
		}
		else
		{
			TempByte = vlByteArray[offset];
			offset++;

			if ((TempByte == 0xFE) && (vlByteArray[offset] != 0xFE) && includeFERepeats)
			{
				byte repeatFirstByte = vlByteArray[offset];
				offset++;

				unsigned short repeatDistanceFromBeginningMarker = ((repeatFirstByte << 8) | vlByteArray[offset]);
				offset++;
				byte repeatCount = vlByteArray[offset];
				offset++;

				altPattern = new byte[repeatCount];
				for (int copy = ((offset - 4) - repeatDistanceFromBeginningMarker); copy < (((offset - 4) - repeatDistanceFromBeginningMarker) + repeatCount); copy++)
				{
					altPattern[copy - ((offset - 4) - repeatDistanceFromBeginningMarker)] = vlByteArray[copy];
				}
				altOffset = 0;
				altLength = repeatCount;

				TempByte = altPattern[altOffset];
				altOffset++;
			}
			else if ((TempByte == 0xFE) && (vlByteArray[offset] == 0xFE) && includeFERepeats)
			{
				// skip duplicate FEs
				offset++;
			}

			if ((altOffset == altLength) && (altPattern != NULL))
			{
				delete [] altPattern;
				altPattern = NULL;
				altOffset = 0;
				altLength = 0;
			}
		}
        if ((TempByte >> 7) == 0x1)
        {
            VLVal += TempByte;
            VLVal = VLVal << 8; //Shift to next byte in VLVal.
        }
        else
        {
            VLVal += TempByte;
            break;
        } 
    }
    
	original = VLVal;

	unsigned long Vlength = 0;

	for (int c = 0, a = 0; ;c += 8, a+= 7)
    {
       
        Vlength += (((VLVal >> c) & 0x7F) << a);
		if (c == 24)
			break;
    }
    return Vlength;
}

void WriteVLBytes(FILE* outFile, unsigned long value, unsigned long length, bool includeFERepeats)
{
	byte tempByte;
	if (length == 1)
	{
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else if (length == 2)
	{
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else if (length == 3)
	{
		tempByte = (value >> 16) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else
	{
		tempByte = (value >> 24) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
}

unsigned long ReturnVLBytes(unsigned long value, unsigned long& length)
{
	byte subValue1 = (value >> 21) & 0x7F;
	byte subValue2 = (value >> 14) & 0x7F;
	byte subValue3 = (value >> 7) & 0x7F;
	byte subValue4 = (value >> 0) & 0x7F;

	if (subValue1 > 0)
	{
		unsigned long newValue = 0x80808000;
		newValue |= (subValue1 << 24);
		newValue |= (subValue2 << 16);
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 4;
		return newValue;
	}
	else if (subValue2 > 0)
	{
		unsigned long newValue = 0x00808000;
		newValue |= (subValue2 << 16);
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 3;
		return newValue;
	}
	else if (subValue3 > 0)
	{
		unsigned long newValue = 0x00008000;
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 2;
		return newValue;
	}
	else
	{
		length = 1;
		return value;
	}
}

void GEMidiToMidi(byte* inputMID,
                  int inputSize,
                  std::string outFileName,
                  int& numberInstruments,
                  bool& hasLoopPoint,
                  int& loopStart,
                  int& loopEnd,
                  bool extendTracksToHighest,
                  bool usePitchBendSensitity,
                  int pitchBendSensitity)
{
	numberInstruments = 0;
   FILE* outFile = fopen(outFileName.c_str(), "wb");

   if (outFile == NULL) {
      std::cout << "Error outputting file" << std::endl;
      return;
   }

   unsigned long lengthHeader = 0x44;

   /* Parse Midi */
   int trackSize = 0;
   for (int i = 0; i < (lengthHeader - 4); i+=4) // ignore last 00000180
   {
      unsigned long offset = CharArrayToLong(&inputMID[i]);
      if (offset != 0)
         trackSize++;
   }

   unsigned long tempLong = Flip32Bit(0x4D546864);
   fwrite(&tempLong, 1 ,4 , outFile);
   tempLong = Flip32Bit(0x00000006);
   fwrite(&tempLong, 1 ,4 , outFile);
   tempLong = Flip32Bit(0x00010000 | trackSize);
   fwrite(&tempLong, 1 ,4 , outFile);

   unsigned long division = CharArrayToLong(&inputMID[0x40]);

   unsigned short tempShort = division;
   tempShort = Flip16Bit(tempShort);
   fwrite(&tempShort, 1 ,2 , outFile);

   int counterTrack = 0;

   int highestTrackLength = 0;
   for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
   {
      unsigned long absoluteTime = 0;
      unsigned long offset = CharArrayToLong(&inputMID[iii]);
      int position = offset;	

      if (position != 0)
      {
         int previousEventValue = 0;

         std::map<int, int> loopEndsWithCount;

         byte* repeatPattern = NULL;
         byte altOffset = 0;
         byte altLength = 0;

         bool endFlag = false;

         while ((position < inputSize) && !endFlag)
         {
            int timePosition = position;

            unsigned long original;
            unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

            absoluteTime += timeTag;

            if (absoluteTime > highestTrackLength)
               highestTrackLength = absoluteTime;


            int vlLength = 0;
            byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

            bool statusBit = false;

            if (eventVal < 0x80)
            {
               // continuation
               statusBit = true;
            }
            else
            {
               statusBit = false;
            }

            if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
            {
               byte subType;
               if (statusBit)
                  subType = eventVal;
               else
                  subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

               if (subType == 0x51) // tempo
               {
                  int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

               }
               else if (subType == 0x2D) // end loop
               {
                  byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
                  
                  if ((loopCount == 0xFF) || (loopCount == 0x00))
                  {
                     break;
                  }
                  else
                  {
                     std::map<int, int>::iterator it = loopEndsWithCount.find(position);
                     if (it != loopEndsWithCount.end())
                     {
                        int countLeft = it->second;

                        if (countLeft == 0)
                        {
                           loopEndsWithCount.erase(it);
                        }
                        else
                        {
                           loopEndsWithCount[position] = (countLeft - 1);

                           if (repeatPattern == NULL)
                           {
                              position = position - offsetToBeginningLoop;
                           }
                           else
                           {
                              loopEndsWithCount.erase(it);
                           }
                        }
                     }
                     else
                     {
                        loopEndsWithCount[position] = loopCount - 1;

                        if (repeatPattern == NULL)
                        {
                           position = position - offsetToBeginningLoop;
                        }
                        else
                        {
                           loopEndsWithCount.erase(it);
                        }
                     }
                  }
               }
               else if (subType == 0x2E) // start loop
               {
                  byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
               }
               else if (subType == 0x2F)
               {
                  endFlag = true;
               }

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
            {
               byte curEventVal;

               byte noteNumber;
               if (statusBit)
               {
                  noteNumber = eventVal;
                  curEventVal = previousEventValue;
               }
               else
               {
                  noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  curEventVal = eventVal;
               }
               byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

               unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
            {
               std::string controllerTypeText = "";
               byte controllerType;
               
               if (statusBit)
               {
                  controllerType = eventVal;
                  previousEventValue;
               }
               else
               {
                  controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  eventVal;
               }
               byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
            {
               byte instrument;
               if (statusBit)
               {
                  instrument = eventVal;
                  previousEventValue;
               }
               else
               {
                  instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  eventVal;
               }

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
            {
               byte amount;
               if (statusBit)
               {
                  amount = eventVal;
                  previousEventValue;
               }
               else
               {
                  amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  eventVal;
               }

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
            {
               byte valueLSB;
               if (statusBit)
               {
                  valueLSB = eventVal;
                  previousEventValue;
               }
               else
               {
                  valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  eventVal;
               }
               
               byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (eventVal == 0xFE) // repeat operation
            {
               // should not be here...

               // no prev event set
            }
            else
            {
               
            }
         }
      }
   }

   /* Part 2 */
   for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
   {
      unsigned long absoluteTime = 0;

      int trackEventCountSub = 0;
      TrackEvent* trackEventsSub = new TrackEvent[0x30000];

      for (int j = 0; j < 0x30000; j++)
      {
         trackEventsSub[j].contents = NULL;
         trackEventsSub[j].obsoleteEvent = false;
         trackEventsSub[j].deltaTime = 0;
         trackEventsSub[j].absoluteTime = 0;

      }

      unsigned long offset = CharArrayToLong(&inputMID[iii]);
      
      int position = offset;	

      if (position != 0)
      {
         tempLong = Flip32Bit(0x4D54726B);
         fwrite(&tempLong, 1 ,4 , outFile);

         int previousEventValue = 0;

         std::map<int, int> loopEndsWithCount;
         std::vector<int> loopNumbers;

         byte* repeatPattern = NULL;
         byte altOffset = 0;
         byte altLength = 0;

         bool endFlag = false;

         if (usePitchBendSensitity)
         {
            //https://www.midikits.net/midi_analyser/pitch_bend.htm

            trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
            trackEventsSub[trackEventCountSub].contentSize = 2;
            trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
            
            trackEventsSub[trackEventCountSub].contents[0] = 0x64;
            trackEventsSub[trackEventCountSub].contents[1] = 0x00;

            trackEventCountSub++;


            trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
            trackEventsSub[trackEventCountSub].contentSize = 2;
            trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
            
            trackEventsSub[trackEventCountSub].contents[0] = 0x65;
            trackEventsSub[trackEventCountSub].contents[1] = 0x00;

            trackEventCountSub++;


            trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
            trackEventsSub[trackEventCountSub].contentSize = 2;
            trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
            
            trackEventsSub[trackEventCountSub].contents[0] = 0x06;
            if (pitchBendSensitity > 0x18)
               pitchBendSensitity = 0x18;
            trackEventsSub[trackEventCountSub].contents[1] = pitchBendSensitity;

            trackEventCountSub++;

            trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
            trackEventsSub[trackEventCountSub].contentSize = 2;
            trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
            
            trackEventsSub[trackEventCountSub].contents[0] = 0x64;
            trackEventsSub[trackEventCountSub].contents[1] = 0x7F;

            trackEventCountSub++;


            trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
            trackEventsSub[trackEventCountSub].contentSize = 2;
            trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
            
            trackEventsSub[trackEventCountSub].contents[0] = 0x65;
            trackEventsSub[trackEventCountSub].contents[1] = 0x7F;

            trackEventCountSub++;
         }

         while ((position < inputSize) && !endFlag)
         {
            if (extendTracksToHighest)
            {
               if (absoluteTime >= highestTrackLength)
               {
                  trackEventsSub[trackEventCountSub].absoluteTime = highestTrackLength;
                  trackEventsSub[trackEventCountSub].deltaTime = (highestTrackLength - absoluteTime);

                  trackEventsSub[trackEventCountSub].type = 0xFF;
                  trackEventsSub[trackEventCountSub].contentSize = 2;
                  trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
                  
                  trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
                  trackEventsSub[trackEventCountSub].contents[1] = 0x0;

                  trackEventCountSub++;

                  endFlag = true;

                  break;
               }
            }


            if (trackEventCountSub >= 0x30000)
            {
               for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
               {
                  if (trackEventsSub[eventCount].contents != NULL)
                  {
                     delete [] trackEventsSub[eventCount].contents;
                     trackEventsSub[eventCount].contents = NULL;
                  }
               }

               delete [] trackEventsSub;
               return;
            }

            int timePosition = position;

            unsigned long original;
            // trackEventsSub[trackEventCountSub].deltaTime is for loops
            unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

            if (extendTracksToHighest)
            {
               if ((absoluteTime + timeTag) > highestTrackLength)
               {
                  trackEventsSub[trackEventCountSub].absoluteTime = highestTrackLength;
                  trackEventsSub[trackEventCountSub].deltaTime = (highestTrackLength - absoluteTime);

                  trackEventsSub[trackEventCountSub].type = 0xFF;
                  trackEventsSub[trackEventCountSub].contentSize = 2;
                  trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
                  
                  trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
                  trackEventsSub[trackEventCountSub].contents[1] = 0x0;

                  trackEventCountSub++;

                  endFlag = true;

                  break;
               }
            }

            trackEventsSub[trackEventCountSub].deltaTime += timeTag;

            absoluteTime += timeTag;
            trackEventsSub[trackEventCountSub].absoluteTime = absoluteTime;

            int vlLength = 0;
            byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

            bool statusBit = false;

            if (eventVal < 0x80)
            {
               // continuation
               statusBit = true;
            }
            else
            {
               statusBit = false;
            }

            if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
            {
               byte subType;
               if (statusBit)
                  subType = eventVal;
               else
                  subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

               if (subType == 0x51) // tempo
               {
                  int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

                  trackEventsSub[trackEventCountSub].type = 0xFF;
                  trackEventsSub[trackEventCountSub].contentSize = 5;
                  trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
                  
                  trackEventsSub[trackEventCountSub].contents[0] = 0x51;
                  trackEventsSub[trackEventCountSub].contents[1] = 0x3;
                  trackEventsSub[trackEventCountSub].contents[2] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
                  trackEventsSub[trackEventCountSub].contents[3] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
                  trackEventsSub[trackEventCountSub].contents[4] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

                  trackEventCountSub++;

                  
                  int MICROSECONDS_PER_MINUTE = 60000000;
                  float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
               }
               else if (subType == 0x2D) // end loop
               {
                  int loopNumber = 0;
                  if (loopNumbers.size() > 0)
                  {
                     loopNumber = loopNumbers.back();
                     loopNumbers.pop_back();
                  }

                  // Fake loop end, controller 103
                  trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
                  trackEventsSub[trackEventCountSub].contentSize = 2;
                  trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
                  trackEventsSub[trackEventCountSub].contents[0] = 103;
                  trackEventsSub[trackEventCountSub].contents[1] = loopNumber;
                  trackEventCountSub++;

                  byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
                  
                  if ((loopCount == 0xFF) || (loopCount == 0x00))
                  {
                     hasLoopPoint = true;
                     loopEnd = absoluteTime;

                     if (extendTracksToHighest)
                     {
                        if (repeatPattern == NULL)
                        {
                           position = position - offsetToBeginningLoop;
                        }
                     }
                  }
                  else
                  {
                     std::map<int, int>::iterator it = loopEndsWithCount.find(position);
                     if (it != loopEndsWithCount.end())
                     {
                        int countLeft = it->second;

                        if (countLeft == 0)
                        {
                           loopEndsWithCount.erase(it);
                        }
                        else
                        {
                           loopEndsWithCount[position] = (countLeft - 1);

                           if (repeatPattern == NULL)
                           {
                              position = position - offsetToBeginningLoop;
                           }
                           else
                           {
                              loopEndsWithCount.erase(it);
                           }
                        }
                     }
                     else
                     {
                        loopEndsWithCount[position] = loopCount - 1;

                        if (repeatPattern == NULL)
                        {
                           position = position - offsetToBeginningLoop;
                        }
                        else
                        {
                           loopEndsWithCount.erase(it);
                        }
                     }
                  }
               }
               else if (subType == 0x2E) // start loop
               {
                  byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
                  hasLoopPoint = true;
                  loopStart = absoluteTime;

                  // Fake loop start, controller 102
                  trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
                  trackEventsSub[trackEventCountSub].contentSize = 2;
                  trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
                  trackEventsSub[trackEventCountSub].contents[0] = 102;
                  trackEventsSub[trackEventCountSub].contents[1] = loopNumber;
                  trackEventCountSub++;



                  loopNumbers.push_back(loopNumber);
               }
               else if (subType == 0x2F)
               {
                  trackEventsSub[trackEventCountSub].type = 0xFF;
                  trackEventsSub[trackEventCountSub].contentSize = 2;
                  trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
                  
                  trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
                  trackEventsSub[trackEventCountSub].contents[1] = 0x0;

                  trackEventCountSub++;

                  endFlag = true;
               }

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
            {
               byte curEventVal;

               byte noteNumber;
               if (statusBit)
               {
                  trackEventsSub[trackEventCountSub].type = previousEventValue;
                  noteNumber = eventVal;
                  curEventVal = previousEventValue;
               }
               else
               {
                  trackEventsSub[trackEventCountSub].type = eventVal;
                  noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  curEventVal = eventVal;
               }
               byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

               unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

               

               trackEventsSub[trackEventCountSub].durationTime = timeDuration; // to be filled in
               trackEventsSub[trackEventCountSub].contentSize = 2;
               trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
               trackEventsSub[trackEventCountSub].contents[0] = noteNumber;
               trackEventsSub[trackEventCountSub].contents[1] = velocity;

               trackEventCountSub++;

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
            {
               std::string controllerTypeText = "";
               byte controllerType;
               
               if (statusBit)
               {
                  controllerType = eventVal;
                  trackEventsSub[trackEventCountSub].type = previousEventValue;
               }
               else
               {
                  controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  trackEventsSub[trackEventCountSub].type = eventVal;
               }
               byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

               trackEventsSub[trackEventCountSub].contentSize = 2;
               trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
               trackEventsSub[trackEventCountSub].contents[0] = controllerType;
               trackEventsSub[trackEventCountSub].contents[1] = controllerValue;

               trackEventCountSub++;

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
            {
               byte instrument;
               if (statusBit)
               {
                  instrument = eventVal;
                  trackEventsSub[trackEventCountSub].type = previousEventValue;
               }
               else
               {
                  instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  trackEventsSub[trackEventCountSub].type = eventVal;
               }

               trackEventsSub[trackEventCountSub].contentSize = 1;
               trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
               trackEventsSub[trackEventCountSub].contents[0] = instrument;
               if (instrument >= numberInstruments)
                  numberInstruments = (instrument + 1);

               trackEventCountSub++;

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
            {
               byte amount;
               if (statusBit)
               {
                  amount = eventVal;
                  trackEventsSub[trackEventCountSub].type = previousEventValue;
               }
               else
               {
                  amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  trackEventsSub[trackEventCountSub].type = eventVal;
               }

               trackEventsSub[trackEventCountSub].contentSize = 1;
               trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
               trackEventsSub[trackEventCountSub].contents[0] = amount;

               trackEventCountSub++;

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
            {
               byte valueLSB;
               if (statusBit)
               {
                  valueLSB = eventVal;
                  trackEventsSub[trackEventCountSub].type = previousEventValue;
               }
               else
               {
                  valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
                  trackEventsSub[trackEventCountSub].type = eventVal;
               }
               
               byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

               trackEventsSub[trackEventCountSub].contentSize = 2;
               trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
               trackEventsSub[trackEventCountSub].contents[0] = valueLSB;
               trackEventsSub[trackEventCountSub].contents[1] = valueMSB;

               trackEventCountSub++;

               if (!statusBit)
                  previousEventValue = eventVal;
            }
            else if (eventVal == 0xFE) // repeat operation
            {
               // should not be here...

               // no prev event set
            }
            else
            {
               fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
            }

         }

         for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
         {
            if (trackEventCountSub >= 0x30000)
            {
               fclose(outFile);

               for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
               {
                  if (trackEventsSub[eventCount].contents != NULL)
                  {
                     delete [] trackEventsSub[eventCount].contents;
                     trackEventsSub[eventCount].contents = NULL;
                  }
               }

               delete [] trackEventsSub;
               return;
            }

            TrackEvent trackEvent = trackEventsSub[eventCount];
            if ((trackEvent.type >= 0x90) && (trackEvent.type < 0xA0))
            {
               // need to split out
               if (trackEvent.durationTime > 0)
               {

                  unsigned long shutoffTime = (trackEvent.absoluteTime + trackEvent.durationTime);

                  if (eventCount != (trackEventCountSub - 1))
                  {

                     for (int i = (eventCount+1); i < trackEventCountSub; i++)
                     {
                        if ((trackEventsSub[i].absoluteTime >= shutoffTime) && (i != (trackEventCountSub - 1)))
                        {
                           for (int j = (trackEventCountSub - 1); j >= i; j--)
                           {
                              trackEventsSub[j+1].absoluteTime = trackEventsSub[j].absoluteTime;
                              trackEventsSub[j+1].contentSize = trackEventsSub[j].contentSize;
                              if (trackEventsSub[j+1].contents != NULL)
                              {
                                 delete [] trackEventsSub[j+1].contents;
                                 trackEventsSub[j+1].contents = NULL;
                              }
                              trackEventsSub[j+1].contents = new byte[trackEventsSub[j].contentSize];
                              for (int r = 0; r < trackEventsSub[j].contentSize; r++)
                              {
                                 trackEventsSub[j+1].contents[r] = trackEventsSub[j].contents[r];
                              }
                              trackEventsSub[j+1].deltaTime = trackEventsSub[j].deltaTime;
                              trackEventsSub[j+1].durationTime = trackEventsSub[j].durationTime;
                              trackEventsSub[j+1].obsoleteEvent = trackEventsSub[j].obsoleteEvent;
                              trackEventsSub[j+1].type = trackEventsSub[j].type;
                           }

                           trackEventsSub[i].type = trackEventsSub[eventCount].type;
                           trackEventsSub[i].absoluteTime = shutoffTime;
                           trackEventsSub[i].deltaTime = (trackEventsSub[i].absoluteTime - trackEventsSub[i-1].absoluteTime);
                           trackEventsSub[i].contentSize = trackEventsSub[eventCount].contentSize;
                           trackEventsSub[i].durationTime = 0;


                           if (trackEventsSub[i].contents != NULL)
                           {
                              delete [] trackEventsSub[i].contents;
                           }

                           trackEventsSub[i].contents = new byte[trackEventsSub[i].contentSize];
                           trackEventsSub[i].contents[0] = trackEventsSub[eventCount].contents[0];
                           trackEventsSub[i].contents[1] = 0;

                           trackEventsSub[i+1].deltaTime = (trackEventsSub[i+1].absoluteTime - trackEventsSub[i].absoluteTime);

                           if (trackEventsSub[i].deltaTime > 0xFF000000)
                           {
                              int a =1;
                           }

                           trackEventCountSub++;
                           break;
                        }
                        else if (i == (trackEventCountSub - 1))
                        {
                           trackEventsSub[i+1].absoluteTime = shutoffTime; // move end to end
                           trackEventsSub[i+1].contentSize = trackEventsSub[i].contentSize;
                           if (trackEventsSub[i+1].contents != NULL)
                           {
                              delete [] trackEventsSub[i+1].contents;
                              trackEventsSub[i+1].contents = NULL;
                           }
                           trackEventsSub[i+1].contents = new byte[trackEventsSub[i].contentSize];
                           for (int r = 0; r < trackEventsSub[i].contentSize; r++)
                           {
                              trackEventsSub[i+1].contents[r] = trackEventsSub[i].contents[r];
                           }
                           trackEventsSub[i+1].deltaTime = trackEventsSub[i].deltaTime;
                           trackEventsSub[i+1].durationTime = trackEventsSub[i].durationTime;
                           trackEventsSub[i+1].obsoleteEvent = trackEventsSub[i].obsoleteEvent;
                           trackEventsSub[i+1].type = trackEventsSub[i].type;


                           trackEventsSub[i].type = trackEventsSub[eventCount].type;
                           trackEventsSub[i].absoluteTime = shutoffTime;
                           trackEventsSub[i].deltaTime = (trackEventsSub[i].absoluteTime - trackEventsSub[i - 1].absoluteTime);
                           trackEventsSub[i].contentSize = trackEventsSub[eventCount].contentSize;
                           trackEventsSub[i].durationTime = 0;

                           if (trackEventsSub[i].contents != NULL)
                           {
                              delete [] trackEventsSub[i].contents;
                           }

                           trackEventsSub[i].contents = new byte[trackEventsSub[i].contentSize];
                           trackEventsSub[i].contents[0] = trackEventsSub[eventCount].contents[0];
                           trackEventsSub[i].contents[1] = 0;

                           trackEventsSub[i+1].deltaTime = (trackEventsSub[i+1].absoluteTime - trackEventsSub[i].absoluteTime);

                           trackEventCountSub++;
                           break;
                        }
                     }
                  }
                  else
                  {
                     trackEventsSub[eventCount+1].absoluteTime = shutoffTime; // move end to end
                     trackEventsSub[eventCount+1].contentSize = trackEventsSub[eventCount].contentSize;
                     if (trackEventsSub[eventCount+1].contents != NULL)
                     {
                        delete [] trackEventsSub[eventCount+1].contents;
                        trackEventsSub[eventCount+1].contents = NULL;
                     }
                     trackEventsSub[eventCount+1].contents = new byte[trackEventsSub[eventCount].contentSize];
                     for (int r = 0; r < trackEventsSub[eventCount].contentSize; r++)
                     {
                        trackEventsSub[eventCount+1].contents[r] = trackEventsSub[eventCount].contents[r];
                     }
                     trackEventsSub[eventCount+1].deltaTime = trackEventsSub[eventCount].deltaTime;
                     trackEventsSub[eventCount+1].durationTime = trackEventsSub[eventCount].durationTime;
                     trackEventsSub[eventCount+1].obsoleteEvent = trackEventsSub[eventCount].obsoleteEvent;
                     trackEventsSub[eventCount+1].type = trackEventsSub[eventCount].type;


                     trackEventsSub[eventCount].type = trackEventsSub[eventCount].type;
                     trackEventsSub[eventCount].absoluteTime = shutoffTime;
                     if ((trackEventsSub[eventCount].absoluteTime - trackEventsSub[eventCount - 1].absoluteTime) > 0xFF000000)
                     {
                        int a =1;
                     }
                     trackEventsSub[eventCount].deltaTime = (trackEventsSub[eventCount].absoluteTime - trackEventsSub[eventCount - 1].absoluteTime);
                     trackEventsSub[eventCount].contentSize = trackEventsSub[eventCount].contentSize;
                     trackEventsSub[eventCount].durationTime = 0;
                     trackEventsSub[eventCount].contents = new byte[trackEventsSub[eventCount].contentSize];
                     trackEventsSub[eventCount].contents[0] = trackEventsSub[eventCount].contents[0];
                     trackEventsSub[eventCount].contents[1] = 0;

                     trackEventsSub[eventCount+1].deltaTime = (trackEventsSub[eventCount+1].absoluteTime - trackEventsSub[eventCount].absoluteTime);
                     if (trackEventsSub[eventCount].deltaTime > 0xFF000000)
                     {
                        int a =1;
                     }
                     trackEventCountSub++;
                  }
               }
            }
         }

         
         unsigned long timeOffset = 0;

         unsigned long sizeData = 0;
         byte previousTrackEvent = 0x0;

         
         for (int j = 0; j < trackEventCountSub; j++)
         {
            TrackEvent trackEvent =  trackEventsSub[j];
            if (trackEvent.obsoleteEvent)
            {
               timeOffset += trackEvent.deltaTime;
            }
            else
            {
               unsigned long lengthTimeDelta = 0;
               unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
               timeOffset = 0;

               sizeData += lengthTimeDelta;


               if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
               {
                  sizeData += 1;
               }

               sizeData += trackEvent.contentSize;

               previousTrackEvent = trackEvent.type;
            }
         }

         tempLong = Flip32Bit(sizeData);
         fwrite(&tempLong,1, 4, outFile);

         timeOffset = 0;
         previousTrackEvent = 0x0;
         for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
         {
            TrackEvent trackEvent = trackEventsSub[eventCount];

            if (trackEvent.obsoleteEvent)
            {
               timeOffset += trackEvent.deltaTime;
            }
            else
            {
               unsigned long lengthTimeDelta = 0;
               unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
               timeOffset = 0;
               WriteVLBytes(outFile, timeDelta, lengthTimeDelta, true);

               if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
               {
                  fwrite(&trackEvent.type, 1, 1, outFile);
               }

               fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

               previousTrackEvent = trackEvent.type;
            }
         }
      }
      else
      {
         
      }

      for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
      {
         if (trackEventsSub[eventCount].contents != NULL)
         {
            delete [] trackEventsSub[eventCount].contents;
            trackEventsSub[eventCount].contents = NULL;
         }
      }

      counterTrack++;

      delete [] trackEventsSub;
   }





   fflush(outFile);	
   fclose(outFile);
}

int main() {
   std::vector<byte> inputSeq = LoadBinary("rainbow.bin");
   std::cout << inputSeq.size();

   /* Hard code Mario's Rainbow Castle */
   int numberInstruments = 12;
   bool hasLoopPoint = false;
   int loopStart = 0;
   int loopEnd = 0;
   GEMidiToMidi(inputSeq.data(), inputSeq.size(), "out/rainbow.midi", numberInstruments,
                hasLoopPoint, loopStart, loopEnd, false, false, 0);

   std::cout << "Hello World!\n" << std::endl;
   return 0;
}