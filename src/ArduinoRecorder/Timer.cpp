#include "Timer.h"
#include "Arduino.h"


Timer::Timer()
{
	this->_isRunning = false;
}

Timer::~Timer()
{
}

bool Timer::isExpired() const
{
	return (millis() - this->_startTime) > this->_timeMs;
}

bool Timer::isRunning() const
{
	return this->_isRunning;
}

void Timer::start(unsigned long timeMs) {
	this->_timeMs = timeMs;
	this->_isRunning = true;
	this->_startTime = millis();
}

void Timer::stop() {
	this->_isRunning = false;
}