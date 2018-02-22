#include "ColorSequencer.h"

ColorSequencer::ColorSequencer(unsigned long speed, unsigned short maxBrightness, int minBrightnessSum){
	tick.ton(false);
	tickSpeed		= speed;
	maxBright		= maxBrightness;
	minBrightSum	= minBrightnessSum; 
}

void ColorSequencer::call(bool init) {
	if(init){
		getNew();
		actualColor.r	=	showColor.r;
		actualColor.g	=	showColor.g;
		actualColor.b	=	showColor.b;
	}
	else{
		if(tick.ton(true, tickSpeed)){
			tick.ton(false);
			if(actualColor.r > showColor.r)
				actualColor.r -= 1;
			if(actualColor.r < showColor.r)
				actualColor.r += 1;
			
			if(actualColor.g > showColor.g)
				actualColor.g -= 1;
			if(actualColor.g < showColor.g)
				actualColor.g += 1;
			
			if(actualColor.b > showColor.b)
				actualColor.b -= 1;
			if(actualColor.b < showColor.b)
				actualColor.b += 1;
			
			if(actualColor.r == showColor.r && actualColor.g == showColor.g && actualColor.b == showColor.b) {
				getNew();
			}
		}
	}
}

void ColorSequencer::getNew(){
	do{
		showColor.r		= random(maxBright);
		showColor.g		= random(maxBright);
		showColor.b		= random(maxBright);
	}while( (showColor.r + showColor.g + showColor.b) < minBrightSum);
}