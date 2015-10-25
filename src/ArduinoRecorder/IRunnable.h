#pragma once 

class IRunnable {
public:
	virtual void run() = 0;
	~IRunnable(){}
};