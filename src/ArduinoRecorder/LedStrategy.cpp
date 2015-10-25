#include "Arduino.h"
#include "IRunnable.h"
#include "LedStrategy.h"

// ----- LedStrategy
LedStrategy::LedStrategy(int pin) {
	this->pin = pin;
}

// ----- ConstantLedStrategy
ConstantLedStrategy::ConstantLedStrategy(bool on, int pin) : LedStrategy(pin) {
	this->_on = on;
}

void ConstantLedStrategy::run() {
	int mode = this->_on ? HIGH : LOW;
	digitalWrite(this->pin, mode);
}

// ----- FadeLedStrategy
FadeLedStrategy::FadeLedStrategy(unsigned int speed, int pin) : LedStrategy(pin) {
	this->_speed = speed;
	this->_direction = true;
	this->_lastValue = 0;
	this->_counter = 0;
}

bool FadeLedStrategy::doChange() {
	if (this->_speed == 0) return true;

	unsigned int val = ++this->_counter;

	return (val % this->_speed) == 0;
}

int FadeLedStrategy::next() {
	bool change = this->doChange();
	
	unsigned int val = this->_lastValue;

	if (!change) return val;

	if (this->_direction == true) {
		val += 25;

		if (val >= 255) {
			this->_direction = false;
		}
	}
	else {
		val -= 25;

		if (val <= 0) {
			this->_direction = true;
		}
	}

	val = min(255, max(0, val));
	return this->_lastValue = val;
}

void FadeLedStrategy::run() {
	analogWrite(this->pin, this->next());
}