#pragma once

class DebugLog
{
public:
	static void init();
	static void dumpFreeMemory();

private:
	DebugLog();
	~DebugLog();
};

