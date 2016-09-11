#include "LZWExp.h"
#include "MyLib.h"
#include "BitUnpacker.h"
#include "CodeSet.h"

void sink(void *ptr, char *data, int numBytes) {

}



/*
set up an LZWExp, get data from standard input, and send it to the LZWExp module
*/
int main() {
	LZWExp *exp = malloc(sizeof(LZWExp));
	CodeSet *cs = (CodeSet*)CreateCodeSet(RECYCLE_CODE);
	exp -> dict = cs;

	void *sinkState = 0; //Unknown for now
	
	LZWExpInit(exp, sink, sinkState);
	buInit(&exp -> bitUnpacker);
	UInt hexData = 0;

	while(scanf("%X ", &hexData) != EOF){
		printf("%08X ", hexData);
		//LZWExpDecode(exp, hexData);
	}

	return 1;
}