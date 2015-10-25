#pragma once

#include "LiquidCrystal.h"

class LcdProvider {
public:
	static const int LCD_COL = 16;
	static const int LCD_ROW = 2;

	// Special characters as per datasheet: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf (ROM A00)
	static const char CHAR_ARROW_RIGHT = 0x7E;
	static const char CHAR_SOLID_BLOCK = 0xFF;
	static const char CHAR_BLOCK = 0xDB;

public:
	static LiquidCrystal* instance() {
		static LiquidCrystal lcd(3, 5, 6, 7, 8, 12);
		
		lcd.begin(LcdProvider::LCD_COL, LcdProvider::LCD_ROW);

		return &lcd;
	}
};