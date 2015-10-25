#include "Arduino.h"
#include "Assert.h"
#include "Menu.h"
#include "LcdProvider.h"
#include "Assert.h"

/// --------- MenuItem
MenuItem::MenuItem(const char* text)
{
	int len = strlen(text);

	this->_text = text;

	this->_needsScrolling = (len + MenuItem::PADDING_LEFT /* Marker */) > LcdProvider::LCD_COL;

	this->_lastState = MenuItemState_Active;
	this->_callback = NULL;

	if (!this->_needsScrolling) {
		this->_scrollText = const_cast<char*>(text);
	}
	else {
		// calc offsets
		this->_scrollOffset = 0;
		this->_overflowCount = len - MenuItem::MAX_TEXT_SIZE;

		const size_t arrSize = MenuItem::TEXT_SIZE_BUFFER_LENGTH;
		const size_t arrByteSize = arrSize * sizeof(char);

		// sub string
		this->_scrollText = new char[arrSize];
		memset(this->_scrollText, 0, arrByteSize);
		strncpy(this->_scrollText, text, arrSize - 1);

		ASSERT_EQUAL(strlen(this->_scrollText), MenuItem::MAX_TEXT_SIZE);
	}
}

void MenuItem::setCallback(MenuItemCallback callback) {
	this->_callback = callback;
}

void MenuItem::onActivate(void* arg) {
	if (this->_callback) {
		this->_callback(arg);
	}
}

void MenuItem::render(LiquidCrystal* lcd, MenuItemState state) {
	this->updateTextScrollPosition(state);

	lcd->print(Menu::ProvideStateCharacter(state));
	lcd->print(this->_scrollText);

	this->_lastState = state;
}

void MenuItem::updateTextScrollPosition(MenuItemState state) {
	if (!this->_needsScrolling) {
		return;
	}

	if (!this->_scrollTimer.isRunning()) {
		this->_scrollTimer.start(MenuItem::SCROLL_DELAY_BOUNDARY);
		return;
	}

	if (!this->_scrollTimer.isExpired() && state == this->_lastState) {
		return;
	}

	// increase to next char
	this->_scrollOffset++;

	// handle reset at end
	bool isSelected = state == MenuItemState_Selected;
	if (this->_scrollOffset > this->_overflowCount || !isSelected) {
		this->_scrollOffset = 0;
		this->_scrollTimer.start(MenuItem::SCROLL_DELAY_BOUNDARY);
	}
	else {
		// delay for next scrolling event
		this->_scrollTimer.start(MenuItem::SCROLL_DELAY);
	}

	// reset when state changed
	if (this->_lastState != state) {
		this->_scrollOffset = 0;
	}

	// sub string at beginning
	const size_t arrSize = MenuItem::TEXT_SIZE_BUFFER_LENGTH;
	const size_t arrByteSize = arrSize * sizeof(char);
	memset(this->_scrollText, 0, arrByteSize);

	// ... copies the part of string at offset into memory
	strncpy(this->_scrollText, this->_text + this->_scrollOffset, arrSize-1); 
	ASSERT_EQUAL(strlen(this->_scrollText), MenuItem::MAX_TEXT_SIZE);
}

/// --------- Menu
Menu::Menu() : _swSelect(Menu::PIN_SWSELECT), _swActivate(Menu::PIN_SWACTIVATE)
{
	this->_lcd = LcdProvider::instance();
	this->_menuItems[0] = MenuItem("Start recording piece");
	this->_menuItems[1] = MenuItem("Play recorded piece");
	this->_renderTimer.start(Menu::MENU_RENDER_REFRESH_TIME);
	this->_selectedMenuItemIndex = 0;
	this->_owner = NULL;
}

Menu::~Menu()
{
}

void Menu::run() {
	this->handleInput();
	this->render();
}

void Menu::render() {
	if (!this->_renderTimer.isExpired()) {
		return;
	}

	// reset
	this->_renderTimer.start(Menu::MENU_RENDER_REFRESH_TIME);
	this->_lcd->clear();

	// render each item
	MenuItemState selectedState = this->_swActivate.isPressed() ? MenuItemState_Active : MenuItemState_Selected;
	for (int i = 0; i < Menu::MENUITEM_COUNT; i++)
	{
		bool isSelectedOrActive = (i == this->_selectedMenuItemIndex);
		MenuItemState renderState = isSelectedOrActive ? selectedState : MenuItemState_Default;

		this->_lcd->setCursor(0, i);
		this->_menuItems[i].render(this->_lcd, renderState);
	}
}

void Menu::handleInput() {
	// handling
	this->_swSelect.run();
	this->_swActivate.run();

	// check for changes
	if (this->_swSelect.isJustDepressed()) {
		this->selectNextItem();
	} else if (this->_swActivate.isJustDepressed()) {
		this->onActivateItem();
	}
}

char Menu::ProvideStateCharacter(MenuItemState state){
	switch (state)
	{
	case MenuItemState_Selected:
		return LcdProvider::CHAR_ARROW_RIGHT;
	case MenuItemState_Active:
		return LcdProvider::CHAR_BLOCK;
	case MenuItemState_Default:
		return ' ';
	default:
		return '!';
	}
}

void Menu::selectNextItem() {
	this->_selectedMenuItemIndex = (this->_selectedMenuItemIndex + 1) % Menu::MENUITEM_COUNT;
}

void Menu::onActivateItem() {
	this->_menuItems[this->_selectedMenuItemIndex].onActivate(this->_owner);
}

void Menu::setCallback(void* owner, int index, MenuItemCallback callback) {
	if (this->_owner) {
		ASSERT_EQUAL((int)owner, (int)this->_owner);
	}

	this->_owner = owner;
	this->_menuItems[index].setCallback(callback);
}
