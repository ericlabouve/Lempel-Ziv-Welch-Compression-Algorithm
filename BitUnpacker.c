
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UINT_SIZE 32
#define UINT_MASK 0xFFFFFFFF

typedef unsigned int UInt;

typedef struct {
   UInt curData; //Current data to unpack from
   UInt nextData; //Data to use after curData has no bits left to use
   int bitsLeft; //How many valid bits left are in curData
   int validNext; //A flag to tell us if the nextData is a valid integer to use for data
} BitUnpacker;

void buInit(BitUnpacker *buPtr) {
   buPtr -> curData = 0;
   buPtr -> nextData = 0;
   buPtr -> bitsLeft = 0;
   buPtr -> validNext = 0;
}

//set the UInt as BitPacker's nextData
void buTakeData(BitUnpacker *buPtr, UInt set) {
   buPtr -> nextData = set;
   //buPtr -> validNext = 1;
}

/* Unpack() will return the valid character if it can unpack anything from its 
 * data. Unpack() will return 1 if it successfully unpacked a character, 
 * else will return 0 if it needs buTakeData to get more data. If there 
 * are not enough bits to unpack from curData, it will cross over to use nextData

 * unpack will return 1 if it successfully unpacked the data with the requested size,
 * else will return 0 if it needs buTakeData to get more data.
 */
int unpack(BitUnpacker *buPtr, int size, UInt *ret) {
   int answer = 0;
   
   if(buPtr -> bitsLeft) { //Data is in between bytes
      if(buPtr -> bitsLeft < 0) {
        size = -1 * buPtr -> bitsLeft; //Adjust size for how many bits are actually in our current UInt
        buPtr -> bitsLeft = UINT_SIZE;
      }

      if(size <= buPtr -> bitsLeft) { //Data lies within the UINT       //ABCD

         *ret = buPtr -> curData | buPtr -> nextData >> UINT_SIZE - size; //000A
         buPtr -> nextData <<= size;                                    //BCD0
         buPtr -> bitsLeft -= size;

         buPtr -> curData = 0; //Set back to zero for next *ret assignment

         answer = 1;
      }

      else if(size > buPtr -> bitsLeft) { //Data continues into the next data D000
         //Save incomplete data 00D0 and push left into correct spot
         buPtr -> curData = buPtr -> nextData >> UINT_SIZE - buPtr -> bitsLeft << size - buPtr -> bitsLeft;
         answer = 0; //Get More Data
         buPtr -> bitsLeft -= size; //Will go negative to compensate for the next UInt, bitsLeft
      }
   }

   if(buPtr -> bitsLeft == 0) {
      buPtr -> bitsLeft = UINT_SIZE;
   }

   return answer;
}

