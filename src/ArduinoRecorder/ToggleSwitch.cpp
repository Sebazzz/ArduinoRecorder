#include "Arduino.h"
#include "IRunnable.h"
#include "ToggleSwitch.h"


ToggleSwitch::ToggleSwitch(int pin)
{
	this->_pin = pin;
	this->_isPressed = false;
	this->_wasPressed = false;

	pinMode(this->_pin, INPUT);
	this->_isIniting = (digitalRead(this->_pin) == HIGH);

	Serial.print("Init ToggleSwitch: ");
	Serial.println(this->_pin);
}


ToggleSwitch::~ToggleSwitch()
{
}

void ToggleSwitch::run() {
	int result = digitalRead(this->_pin);

	if (this->_isIniting && result == LOW) {
		this->_isIniting = false;
		return;
	}

	bool isPressed = result == HIGH;
	this->_wasPressed = isPressed != this->_isPressed;
	this->_isPressed = isPressed;
}

bool ToggleSwitch::isChanged() const {
	return this->_wasPressed;
}

bool ToggleSwitch::isPressed() const {
	return this->_isPressed;
}