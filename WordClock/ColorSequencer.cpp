//#define ldrDebug

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
    ldrCalc = (((float)ldr / 1024-1 ) * -1);
    if(ldrCalc < 0.1){ldrCalc = 0.1;}
    #ifdef ldrDebug
        Serial.print("LDR raw: ");
        Serial.print(ldr);
        Serial.print(" LDRSet: ");
        Serial.print(ldrCalc);
        Serial.print(" LDRAct: ");
        Serial.println(ldrStep);
        Serial.print("Red: ");
        Serial.print(calcColor.r);
        Serial.print("  Green: ");
        Serial.print(calcColor.g);
        Serial.print("  Blue: ");
        Serial.println(calcColor.b);
    #endif

    if(ldrCalc < ldrStep)
        ldrStep -= 0.01;
    if(ldrCalc > ldrStep)
        ldrStep += 0.01;

    calcColor.r = (byte)( (float)(actualColor.r * ldrStep));
    calcColor.g = (byte)( (float)(actualColor.g * ldrStep));
    calcColor.b = (byte)( (float)(actualColor.b * ldrStep));
}

void ColorSequencer::getNew(){
rgbWheelPos++;
    if(rgbWheelPos >= 360)
        rgbWheelPos = 0;
    
    if(rgbWheelPos < 85){
        showColor.r = rgbWheelPos * 3;
        showColor.g = 255 - rgbWheelPos * 3;
        showColor.b = 0;
    }
    else if(rgbWheelPos < 170){
        
        showColor.b = rgbWheelPos * 3;
        showColor.r = 255 - rgbWheelPos * 3;
        showColor.g = 0;
    }
    else{
        
        showColor.g = rgbWheelPos * 3;
        showColor.b = 255 - rgbWheelPos * 3;
        showColor.r = 0;
    }
}