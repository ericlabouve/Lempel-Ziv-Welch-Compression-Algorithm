#include "LZWExp.h"
#include "SmartAlloc.h"
#include "BitUnpacker.h"
#include "MyLib.h"
#include <Math.h>


/* Initialize a LZWExp the DataSink to which to send uncompressed symbol 
 * sequences and an anonymous state object for the DataSink.
 */
void LZWExpInit(LZWExp *exp, DataSink sink, void *sinkState) {
   exp -> sink = sink;     /* Data sink to send char sequences to */
   exp -> sinkState = sinkState;   /* Unknown object to send to sink for state */
   exp -> lastCode = STARTING_LASTCODE;      /* Last code that was created */
   exp -> numBits = STARTING_NUMBITS;       /* Number of bits per code currently */
   exp -> maxCode = pow(2, exp -> numBits) - 1;       /* Max code that fits in numBits bits. */
   exp -> EODSeen = 0;
}

/* Break apart compressed data in "bits" into one or more codes and send 
 * the corresponding symbol sequences to the DataSink.  Save any leftover 
 * compressed bits to combine with the bits from the next call of 
 * LZWExpEncode.  Return 0 on success or BAD_CODE if you receive a code not
 * in the dictionary.
 *
 * For this and all other methods, a code is "invalid" if it either could not
 * have been sent (e.g. is too high) or if it is a nonzero code following
 * the detection of an EOD code.
 */
int LZWExpDecode(LZWExp *exp, UInt bits) {
	UInt code;
	buTakeData(&exp -> bitUnpacker, bits);
	while(unpack(&exp -> bitUnpacker, exp -> numBits, &code)){
		printf("%X ", code);
	}
	
	return 1;
}

/* Mark end of decoding.  Returns 0 if all OK, or NO_EOD
 * if no terminating EOD code has been found 
 */
int LZWExpStop(LZWExp *exp) {
	return 0;
}

/* Free all storage associated with LZWExp (not the sinkState, though,
 * which is "owned" by the caller.  Must be called even if LZWExpInit
 * returned an error.
 */
void LZWExpDestruct(LZWExp *exp) {

}
