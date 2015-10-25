#pragma 

#include "Arduino.h"
#include "LiquidCrystal.h"
#include "DebugLog.h"
#include "Timer.h"
#include "Storage.h"
#include "Vector.h"

class StepPlayer {
private:
	StepCollection* _steps;
	LiquidCrystal* _lcd;

	Step _current;
	int _index;
	Timer _timer;
	Timer _currentStepTimer;

	bool checkStepState();
	void displayCurrentPiece();

public:
	void play();
	void reset();

	StepPlayer(LiquidCrystal* lcd, StepCollection* steps);
	~StepPlayer();
};

class StepRecorder
{
private:
	StepCollection* _steps;
	LiquidCrystal* _lcd;

	static const int BUTTON_PIN = 4;
	static const int PITCH_PIN = 0;
	static const int PITCH_DIFF_THRESHOLD = 25;
	static const int MIN_PIECE_TIME = 100;
	static const int TIMING_NOT_STARTED = 0;

	unsigned long _startTime;
	unsigned long _currentPitch;
	bool _isRecording;

private:
	void buttonPressed();
	void buttonDepressed();

	void startRecordPiece();
	void finishRecordPiece();

public:
	StepRecorder(LiquidCrystal* lcd, StepCollection* steps);
	~StepRecorder();

	void record();
	void reset();
};


