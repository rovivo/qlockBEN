#include "Input.h"
#include "Arduino.h"

Input::Input(short _pinBtnUp, short _pinBtnMode, short _pinBtnDown){
	pin_Up		= _pinBtnUp;
	pin_Mode	= _pinBtnMode;
	pin_Down	= _pinBtnDown;
}

void Input::setup(){
	pinMode(pin_Up, INPUT_PULLUP);
	pinMode(pin_Mode, INPUT_PULLUP);
	pinMode(pin_Down, INPUT_PULLUP);
}

void Input::call(){
	actUp	= !digitalRead(pin_Up);
	actMode	= !digitalRead(pin_Mode);
	actDown	= !digitalRead(pin_Down);
	
	fpUp	=  actUp   and !oldUp;
	fpMode	=  actMode and !oldMode;
	fpDown	=  actDown and !oldDown;
	fnUp	= !actUp   and  oldUp;
	fnMode	= !actMode and  oldMode;
	fnDown	= !actDown and  oldDown;
	
	oldUp	= actUp;
	oldMode	= actMode;
	oldDown	= actDown;
}

bool Input::clickUp(bool fn){
	if(fn)
		return fpUp;
	else
		return fnUp;
}

bool Input::clickMode(bool fn){
	if(fn)
		return fpMode;
	else
		return fnMode;
}

bool Input::clickDown(bool fn){
	if(fn)
		return fpDown;
	else
		return fnDown;
}
bool Input::buttonUp(){
	return actUp;
}

bool Input::buttonMode(){
	return actMode;
}

bool Input::buttonDown(){
	return actDown;
}

bool Input::doReset(){
	if(actUp and actDown and fpMode)
		return true;
	else
		return false;
}
