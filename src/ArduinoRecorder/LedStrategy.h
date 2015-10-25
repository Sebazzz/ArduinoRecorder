#pragma once
#include "IRunnable.h"

class LedStrategy : public IRunnable {
protected:
	LedStrategy(int pin);
	int pin;

public:
	virtual void run() = 0;
};

class ConstantLedStrategy : public LedStrategy {
private:
	bool _on;

public:
	ConstantLedStrategy(bool on, int pin);

	virtual void run();
};

class FadeLedStrategy : public LedStrategy {
private:
	unsigned int _speed;
	unsigned int _counter;
	unsigned int _lastValue;
	bool _direction;

	int next();
	bool doChange();

public:
	FadeLedStrategy(unsigned int speed, int pin);

	virtual void run();
};
