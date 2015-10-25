#pragma once

#include "LcdProvider.h"
#include "ToggleSwitch.h"
#include "Timer.h"

enum MenuItemState {
	MenuItemState_Default,
	MenuItemState_Selected,
	MenuItemState_Active
};

typedef void(*MenuItemCallback)(void* owner);

class MenuItem {
protected: 
	static const short SCROLL_DELAY = 750;
	static const short SCROLL_DELAY_BOUNDARY = 1250;
	static const char PADDING_LEFT = 1;
	static const size_t MAX_TEXT_SIZE = LcdProvider::LCD_COL - MenuItem::PADDING_LEFT;
	static const size_t TEXT_SIZE_BUFFER_LENGTH = MenuItem::MAX_TEXT_SIZE + 1;

	const char* _text;

	char* _scrollText;
	Timer _scrollTimer;
	bool _needsScrolling;

	unsigned char _scrollOffset;
	unsigned char _overflowCount;

	MenuItemState _lastState;
	MenuItemCallback _callback;

private:
	void updateTextScrollPosition(MenuItemState state);

public:
	MenuItem(){}
	MenuItem(const char* text);
	void setCallback(MenuItemCallback callback);
	void onActivate(void* arg);
	void render(LiquidCrystal* lcd, MenuItemState state);
};

class Menu
{
private:
	static const int MENUITEM_COUNT = 2;
	static const int PIN_SWACTIVATE = 2;
	static const int PIN_SWSELECT = 4;
	static const int MENU_RENDER_REFRESH_TIME = 150;

	MenuItem _menuItems[MENUITEM_COUNT];
	LiquidCrystal* _lcd;
	int _selectedMenuItemIndex;

	ToggleSwitch _swSelect;
	ToggleSwitch _swActivate;
	Timer _renderTimer;
	void* _owner;

private:
	void render();
	void handleInput();
	void selectNextItem();
	
	void onActivateItem();

public:
	Menu();
	~Menu();

	static char ProvideStateCharacter(MenuItemState state);
	
	void run();
	void setCallback(void* owner, int index, MenuItemCallback callback);
};

