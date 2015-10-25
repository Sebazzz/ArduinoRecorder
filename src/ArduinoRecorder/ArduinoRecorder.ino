#include "Assert.h"
#include "DebugLog.h"
#include "Speaker.h"
#include "Recorder.h"

#include "LiquidCrystal.h"
#include "LcdProvider.h"

Recorder recorder;

void setup() {
	digitalWrite(LED_BUILTIN, LOW);

	LcdProvider::instance();

	DebugLog::init();
	Speaker::init();

	recorder.init();
}

void loop() {
	recorder.run();
}
