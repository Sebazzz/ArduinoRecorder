#include "Storage.h"
#include "StepRecorder.h"
#include "Speaker.h"
#include "LcdProvider.h"

// ----- StepRecorder
StepRecorder::StepRecorder(LiquidCrystal* lcd, StepCollection* steps)
{
	pinMode(StepRecorder::BUTTON_PIN, INPUT);

	this->_isRecording = false;
	this->_startTime = StepRecorder::TIMING_NOT_STARTED;
	this->_steps = steps;

	this->_lcd = lcd;
	this->_lcd->clear();
	this->_lcd->print("Recording...");
	this->_lcd->setCursor(0, 1);
}


StepRecorder::~StepRecorder()
{
}

void StepRecorder::record()
{
	if (digitalRead(StepRecorder::BUTTON_PIN) == LOW) {
		this->buttonDepressed();
	}
	else {
		this->buttonPressed();
	}
}

void StepRecorder::reset()
{
	if (this->_isRecording == false) return;
	this->finishRecordPiece();
}

void StepRecorder::buttonPressed() {
	if (!this->_isRecording) {
		this->startRecordPiece();
	}
}

void StepRecorder::buttonDepressed(){
	if (this->_startTime != StepRecorder::TIMING_NOT_STARTED) {
		this->finishRecordPiece();
	}
}

void StepRecorder::startRecordPiece(){
	this->_isRecording = true;
	this->_startTime = millis();

	Serial.print("Recording... ");

	this->_currentPitch = analogRead(StepRecorder::PITCH_PIN);
	Speaker::tone(this->_currentPitch);
}

void StepRecorder::finishRecordPiece(){
	// add step
	unsigned long currentTime = millis();
	Step s;
	s.pitch = this->_currentPitch;
	s.length = currentTime - this->_startTime;

	if (s.length < StepRecorder::MIN_PIECE_TIME) {
		Serial.println("Piece too short; ignoring.");
	}
	else {
		if (this->_steps->size() >= LcdProvider::LCD_COL) {
			this->_lcd->autoscroll();
		}

		// reg
		this->_steps->addItem(s);

		char text[2] = { 0 };
		text[0] = (char)map(s.pitch, 0, 1023, (int) 'A', (int) 'Z');
		this->_lcd->print(*text);

		Serial.print("End recording ");
		Serial.print(s.length);
		Serial.println(" ms");

	}
	
	this->_isRecording = false;
	this->_startTime = StepRecorder::TIMING_NOT_STARTED;

	Speaker::noTone();
}

// ----- StepPlayer
StepPlayer::StepPlayer(LiquidCrystal* lcd, StepCollection* steps) {
	Serial.print("Player: Initializing with ");
	Serial.print(steps->size());
	Serial.println(" steps");

	this->_steps = steps;
	this->_index = -1;
	this->_lcd = lcd;

	lcd->clear();
	if (steps->size() > 0) {
		lcd->print("Playing...");
		lcd->setCursor(0, 1);
	}
	else {
		lcd->print("Nothing recorded.");
		lcd->setCursor(0, 1);
		lcd->print("Press key to return...");
	}
}

StepPlayer::~StepPlayer() {

}

void StepPlayer::displayCurrentPiece() {
	if (this->_index >= LcdProvider::LCD_COL) {
		this->_lcd->autoscroll();
	}

	char text[2] = { 0 };
	text[0] = (char)map(this->_current.pitch, 0, 1023, (int) 'A', (int) 'Z');
	this->_lcd->print(*text);
}

bool StepPlayer::checkStepState() {
	if (this->_steps->size() == 0){
		return false;
	}

	// first piece
	if (this->_index == -1) {
		this->_index = 0;

		goto playCurrentPiece;
	}

	// determine piece completeness
	if (this->_currentStepTimer.isRunning() && !this->_currentStepTimer.isExpired()) {
		return true;
	}

	// determine play completeness
	this->_index++;
	if (this->_index >= this->_steps->size()) {
		return false;
	}

playCurrentPiece:
	// next piece
	this->_current = this->_steps->getItem(this->_index);
	this->displayCurrentPiece();

	Speaker::tone(this->_current.pitch);
	this->_currentStepTimer.start(this->_current.length);

	return true;
}

void StepPlayer::play() {
	if (this->_steps->size() == 0){
		return;
	}

	while (this->_currentStepTimer.isRunning() && !this->_currentStepTimer.isExpired());

	if (!this->checkStepState()) {
		this->reset();
	}
}

void StepPlayer::reset() {
	Speaker::noTone();

	// restart piece
	if (this->_index >= 0) {
		if (!this->_timer.isRunning()) {
			Serial.println("Player: Playing done");
			this->_timer.start(2000);
			return;
		}

		// isRunning
		if (!this->_timer.isExpired()) {
			return;
		}

		this->_lcd->noAutoscroll();
		this->_lcd->clear();
		this->_lcd->print("Playing...");
		this->_lcd->setCursor(0, 1);

		this->_index = -1;
		this->_timer.stop();
	}
}