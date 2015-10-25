#pragma once

class Speaker
{
private:
	static const unsigned int PITCH_MAX = 1023;
	static const unsigned int PITCH_MIN = 0;
	static const int SPEAKER_PIN = 13;

private:
	Speaker();
	~Speaker();

public:
	static void init();

	// Tone: 0 - 1023
	static void tone(int t);
	static void noTone();
};

