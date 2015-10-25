#include "Speaker.h"
#include "Arduino.h"


Speaker::Speaker()
{
}

Speaker::~Speaker()
{
}

void Speaker::init()
{
	pinMode(Speaker::SPEAKER_PIN, OUTPUT);
}

void Speaker::tone(int t)
{
	int pitch = map(t, Speaker::PITCH_MIN, Speaker::PITCH_MAX, 20, 3000);

	::tone(Speaker::SPEAKER_PIN, pitch);
}

void Speaker::noTone() {
	::noTone(Speaker::SPEAKER_PIN);
}