#include "Timer.h"


Timer::Timer()
{
	timestamp	= millis();
	state		= false;
}

bool Timer::ton(bool in, unsigned long time)
{
	bool out=false;
	if(in) {
		unsigned long ms=millis();
		if(ms < time)
			return false;
			
		if(!state)
			timestamp = ms;
		if((ms - time) > timestamp)
			out = true;
	}
	else
		out = false;
	state = in;
	return out;
}

