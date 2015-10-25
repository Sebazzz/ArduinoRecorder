#include "Arduino.h"
#include "DebugLog.h"
#include "MemoryFree.h"

DebugLog::DebugLog()
{
}

DebugLog::~DebugLog()
{
}

void DebugLog::init() {
	Serial.begin(9600);
	Serial.println();
	Serial.println("Recorder v1");
}

void DebugLog::dumpFreeMemory() {
	Serial.print("Free memory: ");
	Serial.println(freeMemory());
}