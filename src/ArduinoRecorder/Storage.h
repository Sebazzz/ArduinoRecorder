#pragma once

#include "Vector.h"

struct Step {
	unsigned int length;
	/* Pitch: 0 - 1023*/
	unsigned int pitch;
};

typedef Vector<Step> StepCollection;

class Storage
{
private:
	StepCollection _steps;

public:
	Storage();
	~Storage();

	StepCollection* getTemp();
};

