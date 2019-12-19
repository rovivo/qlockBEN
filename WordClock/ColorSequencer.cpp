#include "ColorSequencer.h"

ColorSequencer::ColorSequencer(unsigned long speed, unsigned short maxBrightness, int minBrightnessSum){
	tick.ton(false);
	tickSpeed		= speed;
	maxBright		= maxBrightness;
	minBrightSum	= minBrightnessSum; 
}

void ColorSequencer::call(int ldr, bool init) {
	if(init){
		getNew();
		calcColor.r	= actualColor.r	= showColor.r;
		calcColor.g = actualColor.g	= showColor.g;
		calcColor.b = actualColor.b	= showColor.b;	
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

    ldrCalc = ((ldr / 1024-1 ) * -1);
    calcColor.r = (byte)( (float)(actualColor.r * ldrCalc));
    calcColor.g = (byte)( (float)(actualColor.g * ldrCalc));
    calcColor.b = (byte)( (float)(actualColor.b * ldrCalc));

    Serial.print("Red: ");
    Serial.print(calcColor.r);
    Serial.print("  Green: ");
    Serial.print(calcColor.g);
    Serial.print("  Blue: ");
    Serial.println(calcColor.b);
}

void ColorSequencer::getNew(){
	do{
		showColor.r		= random(maxBright);
		showColor.g		= random(maxBright);
		showColor.b		= random(maxBright);
	}while( (showColor.r + showColor.g + showColor.b) < minBrightSum);
}
