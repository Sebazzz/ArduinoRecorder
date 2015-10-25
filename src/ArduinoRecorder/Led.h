#pragma once

#include "LedStrategy.h"

enum LedMode {
	LedMode_Empty,
	LedMode_Idle,
	LedMode_Recording,
	LedMode_Playing
};

class Led : public IRunnable
{
private:
	const int PIN_R = 11;
	const int PIN_G = 9;
	const int PIN_B = 10;

	LedMode _currentMode;

	static void neutralInit(int pin);

	static void setStrategy(LedStrategy** var, LedStrategy* newObj);

	LedStrategy* redStrategy;
	LedStrategy* blueStrategy;
	LedStrategy* greenStrategy;

public:
	Led();
	~Led();

	void init();
	void run();

	void setMode(LedMode mode);
};

