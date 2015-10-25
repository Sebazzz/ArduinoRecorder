#include "Arduino.h"
#include "IRunnable.h"
#include "LedStrategy.h"
#include "Led.h"
#include "IRecorderState.h"
#include "RecorderState.h"

StartupRecorderState::StartupRecorderState(IStateProvider* stateProvider, Storage* storage) : StorageRecorderState(storage) {
	Serial.println("State: Startup");

	this->_stateProvider = stateProvider;
	this->_hasRenderedIntro = false;
	this->_lcd = LcdProvider::instance();
}

StartupRecorderState::~StartupRecorderState()
{
}

void StartupRecorderState::run() {
	this->renderIntro();
}

void StartupRecorderState::setLed(Led& led) {
	led.setMode(LedMode_Empty);
}

void StartupRecorderState::renderIntro() {
	if (this->_hasRenderedIntro) {
		return;
	}

	this->_hasRenderedIntro = true;

	this->_lcd->clear();
	this->_lcd->print("Arduino Recorder v1");
	this->_lcd->setCursor(0, 1);
	this->_lcd->print("By Sebazzz");
	delay(1000);
	this->_lcd->clear();
	this->_stateProvider->setState(new MenuRecorderState(this->_stateProvider, this->_storage));
}

// --------------- MenuRecorderState
MenuRecorderState::MenuRecorderState(IStateProvider* stateProvider, Storage* storage) : StorageRecorderState(storage)
{
	Serial.println("State: Menu");

	this->_stateProvider = stateProvider;
	this->_menu.setCallback(this, 0, (MenuItemCallback)&MenuRecorderState::wrapOnSelectRecordItem);
	this->_menu.setCallback(this, 1, (MenuItemCallback)&MenuRecorderState::wrapOnSelectPlayItem);
}

MenuRecorderState::~MenuRecorderState()
{
}

void MenuRecorderState::wrapOnSelectPlayItem(void* arg) {
	((MenuRecorderState*)arg)->onSelectPlayItem();
}

void MenuRecorderState::wrapOnSelectRecordItem(void* arg) {
	((MenuRecorderState*)arg)->onSelectRecordItem();
}

void MenuRecorderState::onSelectPlayItem() {
	this->_stateProvider->setState(new PlayingRecorderState(this->_stateProvider, this->_storage, this->_storage->getTemp()));
}

void MenuRecorderState::onSelectRecordItem() {
	StepCollection* storage = this->_storage->getTemp();
	storage->clear(true);

	this->_stateProvider->setState(new RecordingRecorderState(this->_stateProvider, this->_storage, storage));
}

void MenuRecorderState::run() {
	this->_menu.run();
}

void MenuRecorderState::setLed(Led& led) {
	led.setMode(LedMode_Idle);
}


// --------------- RecordingRecorderState
RecordingRecorderState::RecordingRecorderState(IStateProvider* stateProvider, Storage* storage, StepCollection* steps) : StorageRecorderState(storage), _swContinue(RecordingRecorderState::PIN_SWCONTINUE)
{
	Serial.println("State: Recording");

	this->_stateProvider = stateProvider;
	this->_stepRecorder = new StepRecorder(LcdProvider::instance(), steps);
}


RecordingRecorderState::~RecordingRecorderState()
{
	delete this->_stepRecorder;
}

void RecordingRecorderState::run() {
	this->_swContinue.run();
	this->_stepRecorder->record();

	if (this->_swContinue.isJustDepressed()) {
		this->_stepRecorder->reset();
		this->_stateProvider->setState(new MenuRecorderState(this->_stateProvider, this->_storage));
		return;
	}
}

void RecordingRecorderState::setLed(Led& led) {
	led.setMode(LedMode_Recording);
}

// --------------- PlayingRecorderState
PlayingRecorderState::PlayingRecorderState(IStateProvider* stateProvider, Storage* storage, StepCollection* steps) : StorageRecorderState(storage), _swContinue(PlayingRecorderState::PIN_SWCONTINUE)
{
	Serial.println("State: Playing");

	this->_stateProvider = stateProvider;
	this->_stepPlayer = new StepPlayer(LcdProvider::instance(), steps);
}


PlayingRecorderState::~PlayingRecorderState()
{
	delete this->_stepPlayer;
	this->_stepPlayer = NULL;
}

void PlayingRecorderState::run() {
	this->_swContinue.run();
	
	this->_stepPlayer->play();

	if (this->_swContinue.isJustDepressed()) {
		this->_stepPlayer->reset();
		this->_stateProvider->setState(new MenuRecorderState(this->_stateProvider, this->_storage));
		return;
	}
}

void PlayingRecorderState::setLed(Led& led) {
	led.setMode(LedMode_Playing);
}