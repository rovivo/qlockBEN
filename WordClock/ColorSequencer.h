#ifndef ColorSequencer_h
#define ColorSequencer_h

#include <Arduino.h>
#include "Constants.h"
#include "Timer.h"

class ColorSequencer
{
	public:
		ColorSequencer(unsigned long speed=1000, unsigned short maxBrightness=255, int minBrightnessSum=75);
		void call(int ldr=512, bool init=false);
		void getNew();
		float ldrCalc = 0.5;
		uint32_t ColorWheel(uint8_t WheelPos);
		RGB actualColor = {50,50,50};
		RGB calcColor	= {50,50,50};

	private:
		unsigned long	tickSpeed;
		unsigned short	maxBright;
		int				minBrightSum;
		float 			ldrStep;
		uint8_t         rgbWheelPos;
		RGB showColor = {50,50,50};
		Timer tick;

};


#endif