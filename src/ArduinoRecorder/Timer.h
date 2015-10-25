#pragma once
class Timer
{
private:
	unsigned long _timeMs;
	unsigned long _startTime;
	bool _isRunning;

public:
	Timer();
	~Timer();

	bool isExpired() const;
	bool isRunning() const;
	void start(unsigned long timeMs);
	void stop();
};

