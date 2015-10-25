#pragma once

#include "IRecorderState.h"
#include "ToggleSwitch.h"
#include "Recorder.h"
#include "Led.h"
#include "StepRecorder.h"
#include "Menu.h"

class StorageRecorderState : public IRecorderState {
protected:
	Storage* _storage;

public:
	virtual void run() = 0;
	virtual void setLed(Led& led) = 0;

	StorageRecorderState(Storage* storage) {
		this->_storage = storage;
	}

	virtual ~StorageRecorderState(){}
};

class StartupRecorderState : public StorageRecorderState
{
private:
	IStateProvider* _stateProvider;

	LiquidCrystal* _lcd;
	bool _hasRenderedIntro;
	void renderIntro();

public:
	StartupRecorderState(IStateProvider* _stateProvider, Storage* storage);
	virtual ~StartupRecorderState();

	virtual void run();
	virtual void setLed(Led& led);
};


class MenuRecorderState : public StorageRecorderState
{
private: 
	IStateProvider* _stateProvider;
	Menu _menu;

	void onSelectPlayItem();
	void onSelectRecordItem();

	static void wrapOnSelectPlayItem(void* arg);
	static void wrapOnSelectRecordItem(void* arg);

public:
	MenuRecorderState(IStateProvider* _stateProvider, Storage* storage);
	virtual ~MenuRecorderState();

	virtual void run();
	virtual void setLed(Led& led);
};


class RecordingRecorderState : public StorageRecorderState {
private:
	IStateProvider* _stateProvider;
	StepRecorder* _stepRecorder;

	ToggleSwitch _swContinue;

	static const int PIN_SWCONTINUE = 2;

public:
	RecordingRecorderState(IStateProvider* _stateProvider, Storage* storage, StepCollection* _stepCollection);
	virtual ~RecordingRecorderState();

	virtual void run();
	virtual void setLed(Led& led);
};



class PlayingRecorderState : public StorageRecorderState {
private:
	IStateProvider* _stateProvider;
	StepPlayer* _stepPlayer;

	ToggleSwitch _swContinue;

	static const int PIN_SWCONTINUE = 2;

public:
	PlayingRecorderState(IStateProvider* _stateProvider, Storage* storage, StepCollection* steps);
	virtual ~PlayingRecorderState();

	virtual void run();
	virtual void setLed(Led& led);
};
