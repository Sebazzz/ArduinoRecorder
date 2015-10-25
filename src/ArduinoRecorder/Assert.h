#pragma once

#define DEBUG_FAIL(msg, arg) if (true) { \
	Serial.print("FAILED: ");\
	Serial.print(#msg);\
	Serial.print(": ");\
	Serial.print(#arg);\
	Serial.print(" at ");\
	Serial.print(__LINE__);\
	Serial.print(" / ");\
	Serial.println(__FILE__);\
	Serial.println();\
	while(1); \
}

#define ASSERT_EQUAL(_arg1, _arg2) if ((_arg1) != (_arg2)) { \
	Serial.print("ASSERTION FAILED: '");\
	Serial.print(#_arg1);\
	Serial.print(" == ");\
	Serial.print(#_arg2);\
	Serial.print("' at ");\
	Serial.print(__LINE__);\
	Serial.print(" / ");\
	Serial.println(__FILE__);\
	Serial.print(_arg1);\
	Serial.print(" == ");\
	Serial.println(_arg2);\
	Serial.println();\
	while(1); \
}

#define ASSERT_CUSTOM(_cond, _arg1, _arg2) if (!(_cond)) { \
	Serial.print("ASSERTION FAILED: '");\
	Serial.print(#_cond);\
	Serial.print("' at ");\
	Serial.print(__LINE__);\
	Serial.print(" / ");\
	Serial.println(__FILE__);\
	Serial.print("_arg1: ");\
	Serial.println(_arg1);\
	Serial.print("_arg2: ");\
	Serial.println(_arg2);\
	Serial.println();\
		while(1); \
}