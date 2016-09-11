#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "CodeSet.h"
#include "SmartAlloc.h"
#include "MyLib.h"

#define CACHE_MAX 5000

void *CreateCodeSet(int numCodes) {
   //Space for CodeSet Variables
   CodeSet *cs = malloc(sizeof(CodeSet));
   
   //Space for CodeEntry[]
   cs -> array = malloc(numCodes * sizeof(CodeEntry));
   cs -> filled = 0;
   cs -> cacheCount = 0;
   cs -> cacheHead = NULL;
   cs -> cacheTail = NULL;
   return cs;
}

int NewCode(void *codeSet, char val) {
   CodeSet *cs = codeSet;
   
   CodeEntry *cePtr = cs -> array + cs -> filled;
   
   cePtr -> size = 1;
   cePtr -> data = val;
   cePtr -> codeData = NULL; //Will be assigned later
   cePtr -> backPtr = NULL; //At the end of the Linked List
   cePtr -> cachePrev = NULL;
   cePtr -> cacheNext = NULL;
   cePtr -> numBlocks = 0;
   
   return cs -> filled++;
}

int ExtendCode(void *codeSet, int oldCode) {
   CodeSet *cs = codeSet;
   
   //Get pointer to the oldCodeEntry
   CodeEntry *oldCePtr = cs -> array + oldCode;

   //Get pointer to the newCodeEntry
   CodeEntry *newCePtr = cs -> array + cs -> filled;
   
   //Set all fields
   newCePtr -> size = oldCePtr -> size + 1;
   newCePtr -> data = 0;      //1 zero-valued Byte
   newCePtr -> codeData = NULL;
   newCePtr -> backPtr = oldCePtr;
   newCePtr -> cachePrev = NULL;
   newCePtr -> cacheNext = NULL;
   newCePtr -> numBlocks = 0;
   
   return cs -> filled++; 
}

void SetSuffix(void *codeSet, int code, char suffix) {
   CodeSet *cs = codeSet;
   
   CodeEntry *cePtr = cs -> array + code;
   cePtr -> data = suffix;
   
   if (cePtr -> numBlocks) {
      *(cePtr -> codeData + cePtr -> size - 1) = suffix;
   }

}

Code GetCode(void *codeSet, int code) {
   CodeSet *cs = codeSet;
   
   CodeEntry *cePtr = cs -> array + code;
   int ceSize = cePtr -> size;
   CodeEntry *cePtrCpy = cePtr;
   
   //Remove from cache
   if (cePtr -> cachePrev) {
      //Cut out the CodeEntry middle man & reset fields
      cePtr -> cachePrev -> cacheNext = cePtr -> cacheNext;
      cePtr -> cachePrev = NULL;
      cePtr -> cacheNext = NULL;
      
      cs -> cacheCount--;
   }

   //No pointer to the data is available if True
   if (cePtr -> numBlocks == 0) {
      cePtr -> numBlocks++;

      if(cePtr -> codeData == NULL) //Space for the code sequence:
         cePtr -> codeData = malloc(ceSize);

      while (ceSize--) { //Fill in code sequence: (first <- last)
         
         *(cePtrCpy -> codeData + ceSize) = cePtr -> data;
         
         cePtr = cePtr -> backPtr;
   
         //Found a CodeEntry with a pointer to the rest of the code sequence if True
         if (cePtr && cePtr -> numBlocks) {
            memmove(cePtrCpy -> codeData, cePtr -> codeData, cePtr -> size);
            ceSize = 0;
         }
      }
   }

   Code rtnCode = {cePtrCpy -> codeData, cePtrCpy -> size};
   return rtnCode;
}

 /* The user of the CodeSet will be required to call
  * FreeCode to "release" the code when s/he is
  * done with it*/
void FreeCode(void *codeSet, int code) {
   CodeSet *cs = codeSet;
   
   CodeEntry *cePtr = cs -> array + code;
   
   if (cePtr -> numBlocks <= 1) {
      
      //First CodeEntry to be put into cache
      if (cs -> cacheHead == NULL) {
         cs -> cacheHead = cePtr;
         cs -> cacheTail = cePtr;
      }

      else { //Cache is already partly filled
         cs -> cacheHead -> cachePrev = cePtr;
         cePtr -> cacheNext = cs -> cacheHead;
         cs -> cacheHead = cePtr;
      }
      cs -> cacheCount++;
      if (cs -> cacheCount - 1 > CACHE_MAX) {
         
         //Free codeData if not already freed
         if (cs -> cacheTail -> codeData) {
            free(cs -> cacheTail -> codeData);
            cs -> cacheTail -> codeData = NULL;
         }
         
         cs -> cacheTail = cs -> cacheTail -> cachePrev;
         cs -> cacheTail -> cacheNext = NULL;
         cs -> cacheCount--;
      }
      cePtr -> numBlocks--;
   }
}

void DestroyCodeSet(void *codeSet) {
   CodeSet *cs = codeSet;
   CodeEntry *cePtr = cs -> array;
   
   while (cs -> filled--) {
      
      if(cePtr -> codeData) {
         free(cePtr -> codeData);
      }
      cePtr++;
   }
 
   free(cs -> array);
   free(cs);
}
