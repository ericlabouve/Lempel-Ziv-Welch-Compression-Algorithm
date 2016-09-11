#ifndef MYLIB_H
#define MYLIB_H

#define BITS_PER_BYTE 8
#define TRUE 1
#define FALSE 0
#define STARTING_LASTCODE 256
#define STARTING_NUMBITS 9

typedef unsigned char UChar;
typedef unsigned long ULong;
typedef unsigned int UInt;
typedef unsigned short UShort;

typedef struct CodeEntry {
   unsigned int size;
   unsigned int numBlocks; //Number of CodeEntrys which use the same data
   char data;
   char *codeData; //Keeps track of the address from GetCode()
   struct CodeEntry *backPtr;
   struct CodeEntry *cachePrev;
   struct CodeEntry *cacheNext;
} CodeEntry;

typedef struct CodeSet {
   unsigned int filled;
   unsigned int cacheCount;
   CodeEntry *array; //Array of CodeEntrys
   CodeEntry *cacheHead;
   CodeEntry *cacheTail;
} CodeSet;

#endif

