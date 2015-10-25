#include "Storage.h"


Storage::Storage()
{
}


Storage::~Storage()
{
}

Vector<Step>* Storage::getTemp() {
	return &this->_steps;
}