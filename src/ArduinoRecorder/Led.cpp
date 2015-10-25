#include "Arduino.h"
#include "IRunnable.h"
#include "LedStrategy.h"
#include "Led.h"

Led::Led()
{
	this->redStrategy = NULL;
	this->blueStrategy = NULL;
	this->greenStrategy = NULL;
}

Led::~Led()
{
	delete this->blueStrategy;
	delete this->redStrategy;
	delete this->greenStrategy;
}


void Led::init() {
	Led::neutralInit(Led::PIN_R);
	Led::neutralInit(Led::PIN_G);
	Led::neutralInit(Led::PIN_B);

	Led::setStrategy(&this->redStrategy, new ConstantLedStrategy(false, Led::PIN_R));
	Led::setStrategy(&this->blueStrategy, new ConstantLedStrategy(false, Led::PIN_B));
	Led::setStrategy(&this->greenStrategy, new ConstantLedStrategy(false, Led::PIN_G));
}

void Led::run() {
	this->redStrategy->run();
	this->blueStrategy->run();
	this->greenStrategy->run();
}

void Led::neutralInit(int pin){
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

void Led::setMode(LedMode mode) {
	switch (mode)
	{
	case LedMode_Empty:
		Led::setStrategy(&this->redStrategy, new FadeLedStrategy(10, Led::PIN_R));
		Led::setStrategy(&this->blueStrategy, new ConstantLedStrategy(false, Led::PIN_B));
		Led::setStrategy(&this->greenStrategy, new FadeLedStrategy(10, Led::PIN_G));
		break;
	case LedMode_Idle:
		Led::setStrategy(&this->redStrategy, new ConstantLedStrategy(false, Led::PIN_R));
		Led::setStrategy(&this->blueStrategy, new ConstantLedStrategy(true, Led::PIN_B));
		Led::setStrategy(&this->greenStrategy, new ConstantLedStrategy(false, Led::PIN_G));
		break;
	case LedMode_Playing:
		Led::setStrategy(&this->redStrategy, new FadeLedStrategy(10, Led::PIN_R));
		Led::setStrategy(&this->blueStrategy, new ConstantLedStrategy(false, Led::PIN_B));
		Led::setStrategy(&this->greenStrategy, new ConstantLedStrategy(false, Led::PIN_G));
		break;
	case LedMode_Recording:
		Led::setStrategy(&this->redStrategy, new ConstantLedStrategy(false, Led::PIN_R));
		Led::setStrategy(&this->blueStrategy, new ConstantLedStrategy(false, Led::PIN_B));
		Led::setStrategy(&this->greenStrategy, new FadeLedStrategy(true, Led::PIN_G));
		break;
	default:
		abort();
		break;
	}
}

void Led::setStrategy(LedStrategy** var, LedStrategy* newObj) {
	LedStrategy* ptr = *var;
	if (ptr) delete ptr;
	if (newObj) {
		*var = newObj;
	}
	else {
		*var = NULL;
	}
}
