#ifndef ColorSequencer_h
#define ColorSequencer_h

#include <Arduino.h>
#include "Constants.h"
#include "Timer.h"

class ColorSequencer
{
	public:
		ColorSequencer(unsigned long speed=1000, unsigned short maxBrightness=255, int minBrightnessSum=250);
		void call(bool init=false);
		void getNew();
		RGB actualColor = {50,50,50};
		
	private:
		unsigned long	tickSpeed;
		unsigned short	maxBright;
		int				minBrightSum;
		RGB showColor = {50,50,50};
		Timer tick;

};


#endif