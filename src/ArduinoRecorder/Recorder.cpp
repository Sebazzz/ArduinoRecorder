#include "DebugLog.h"
#include "IRunnable.h"
#include "ToggleSwitch.h"
#include "LedStrategy.h"
#include "Led.h"
#include "IRecorderState.h"
#include "RecorderState.h"
#include "Recorder.h"
#include "DebugLog.h"
#include "Arduino.h"

Recorder::Recorder()
{
	this->_currentState = NULL;
}

Recorder::~Recorder()
{
	delete this->_currentState;
	this->_currentState = NULL;
}

void Recorder::init() {
	DebugLog::dumpFreeMemory();

	this->_led.init();

	this->_currentState = new StartupRecorderState(this, &this->_storage);
	this->_currentState->setLed(this->_led);
}

void Recorder::run() {
	this->_led.run();
	this->_currentState->run();
}

void Recorder::setState(IRecorderState* newState) {
	DebugLog::dumpFreeMemory();

	IRecorderState* oldState = this->_currentState;

	delete oldState;
	this->_currentState = newState;
	this->_currentState->setLed(this->_led);

	DebugLog::dumpFreeMemory();
}