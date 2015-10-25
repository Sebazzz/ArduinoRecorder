#pragma once

#include "Menu.h"
#include "Storage.h"
#include "Led.h"

class IRecorderState;


class IStateProvider {

public:
	virtual ~IStateProvider() {}

	virtual void setState(IRecorderState* newState) = 0;
};

class Recorder : public IStateProvider
{
private:
	Led _led;
	IRecorderState* _currentState;
	Storage _storage;

public:
	Recorder();
	~Recorder();

	void init();
	void run();
	virtual void setState(IRecorderState* newState);
};

