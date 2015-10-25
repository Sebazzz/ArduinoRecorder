#pragma once

#include "IRunnable.h"

class ToggleSwitch : public IRunnable
{
private:
	int _pin;
	bool _wasPressed;
	bool _isPressed;
	bool _isIniting;

public:
	ToggleSwitch(int pin);
	~ToggleSwitch();

	bool isChanged() const;
	bool isPressed() const;

	inline bool isJustDepressed() const {
		return this->isChanged() && !this->isPressed();
	}

	virtual void run();
};

