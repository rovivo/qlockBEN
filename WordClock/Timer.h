#ifndef Timer_h
#define Timer_h

#include <Arduino.h>


class Timer
{
	public:
		Timer();
		bool ton(bool in=true, unsigned long time=500);
		bool blink(unsigned long on=500, unsigned long off=1000);
		
	private:
		bool			state;
		unsigned long	timestamp;
		
};


#endif