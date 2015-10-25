#pragma once

#include "Storage.h"
#include "Led.h"

class IRecorderState : public IRunnable {
public:
	virtual void run() = 0;
	virtual void setLed(Led& led) = 0;
	virtual ~IRecorderState(){}
};