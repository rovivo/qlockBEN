#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include "Constants.h"
#include "MyClock.h"
#include "Input.h"
#include "Timer.h"
#include "MatrixFunctions.h"
#include "ColorSequencer.h"
#include "EventHandler.h"
#include "FreeMemory.h"

// ---------- debugs ------------
//#define debug
// DCFDebug in MyClock
// ldrDebug in ColorSequencer


#define PIN_DCF				9

#define PIN_RTC_D			2	// SDA
#define PIN_RTC_C			3	// SCL
#define PIN_RTC_ON			4

#define PIN_BTN_UP			5
#define PIN_BTN_MODE		7
#define PIN_BTN_DOWN		6

#define PIN_LDR				A3
#define PIN_NEOPIXEL		10
#define PIXELS_MAX			114



#define	MODE_CLOCK			0
#define MODE_DCFDIAG		1
#define MODE_TEMP			2
#define MODE_ADDHOUR		3
#define MODE_ADDMIN			4
#define MODE_ADDWDAY		5
#define MODE_ADDDAY			6
#define MODE_ADDMON			7
#define MODE_ADDYEAR		8



#define MODE_INUBIA			10
#define MODE_SCHWEIZ		11
#define MODE_BEN			12
#define MODE_AUSTRIA		13
#define MODE_DAILY			14

#define	MODE_SLEEP			20
#define	MODE_WAKEUP			21


Adafruit_NeoPixel	neo		= Adafruit_NeoPixel(PIXELS_MAX,  PIN_NEOPIXEL,  NEO_GRB + NEO_KHZ800);
MyClock 			clk		= MyClock(PIN_DCF, PIN_RTC_ON, &neo);
Input				inp		= Input(PIN_BTN_UP, PIN_BTN_MODE, PIN_BTN_DOWN, PIN_LDR);
EventHandler		event	= EventHandler();


ColorSequencer		seq		= ColorSequencer(50);

PATTERN 			matrix;

Timer				tmrDiagPrintTime;
Timer				tmrToclock;

void setup(){
	Serial.begin(9600);
	randomSeed(analogRead(0));
	neo.begin();
	clk.setup();
	inp.setup();

	///////////////////////////////////////////////////////
	//# ADD EVENTS (-1 for all values)
	//			MODE			Year	Month	Day		Hour 	Minute	Second		Weekday		// DD.MM.YYYY / HH:II:SS

	//// Montag
	//event.add(	MODE_WAKEUP,	-1,		-1,		-1,		 6,		 0,		 0,			 1		);	// WAKE UP FROM SLEEP
	//event.add(	MODE_SLEEP,		-1,		-1,		-1,		23,		 0,		 0,			 1		);	// GO SLEEP
	//// Dienstag
	//event.add(	MODE_WAKEUP,	-1,		-1,		-1,		 6,		 0,		 0,			 2		);	// WAKE UP FROM SLEEP
	//event.add(	MODE_SLEEP,		-1,		-1,		-1,		23,		 0,		 0,			 2		);	// GO SLEEP
	//// Mittwoch
	//event.add(	MODE_WAKEUP,	-1,		-1,		-1,		 6,		 0,		 0,			 3		);	// WAKE UP FROM SLEEP
	//event.add(	MODE_SLEEP,		-1,		-1,		-1,		23,		 0,		 0,			 3		);	// GO SLEEP
	//// Donnerstag
	//event.add(	MODE_WAKEUP,	-1,		-1,		-1,		 6,		 0,		 0,			 4		);	// WAKE UP FROM SLEEP
	//event.add(	MODE_SLEEP,		-1,		-1,		-1,		23,		 0,		 0,			 4		);	// GO SLEEP
	//// Freitag
	//event.add(	MODE_WAKEUP,	-1,		-1,		-1,		 6,		 0,		 0,			 5		);	// WAKE UP FROM SLEEP
	//// Samstag
	//event.add(	MODE_SLEEP,		-1,		-1,		-1,		 1,		 0,		 0,			 5		);	// GO SLEEP
	//event.add(	MODE_WAKEUP,	-1,		-1,		-1,		 8,		30,		 0,			 6		);	// WAKE UP FROM SLEEP
	//// Sonntag
	//event.add(	MODE_SLEEP,		-1,		-1,		-1,		 1,		 0,		 0,			 6		);	// GO SLEEP
	//event.add(	MODE_WAKEUP,	-1,		-1,		-1,		 9,		 0,		 0,			 7		);	// WAKE UP FROM SLEEP
	//event.add(	MODE_SLEEP,		-1,		-1,		-1,		23,		 0,		 0,			 7		);	// GO SLEEP
	
	event.add(	MODE_SCHWEIZ,	-1,		 8,		 1,		-1,		 0,		25,			-1		);
	
	Serial.println("QlockKerstinRGB");
    Serial.print("compiled: ");
    Serial.print(__DATE__);
	Serial.print(" ");
    Serial.println(__TIME__);
	
	
}

unsigned short state=0;
int action=0;

void loop(){
	// Haupt-Klassen
	clk.call();	// Clock handler
	inp.call();	// Input handler

	// Funktions-Klassen
	seq.call(inp.actBrightness());

	// RTC-Infos bereitstellen
	unsigned short Hour		= clk.getRtcHour();
	unsigned short Minute	= clk.getRtcMinute();
	unsigned short Second	= clk.getRtcSecond();
	unsigned short Year		= clk.getRtcYear();
	unsigned short Month	= clk.getRtcMonth();
	unsigned short Day		= clk.getRtcDay();
	unsigned short Wday		= clk.getRtcWday();

	#ifdef debug
		Serial.print("Time from RTC: ");
		Serial.print(Hour);
		Serial.print(":");
		Serial.print(Minute);
		Serial.print(":");
		Serial.println(Second);
		Serial.print("Date from RTC: ");
		Serial.print(Day);
		Serial.print(".");
		Serial.print(Month);
		Serial.print(".");
		Serial.println(Year);
		Serial.print("LDR: ");
		Serial.println(inp.actBrightness());
	#endif

	// Event-Handler
	action = event.check(Year, Month, Day, Hour, Minute, Second, Wday);
	if(action and state == MODE_CLOCK){
		state = action;
	}
	
	// Inputs & allgemeines
	if(inp.doReset()){
		clk.resetRtc();
		inp.call();	// clears click-functions
	}
	
	
	if(inp.clickMode()){
		state++;
		tmrToclock.ton(false);
		if(state > 8)
			state = 0;
		//Serial.print(F("Mode-Change to: "));
		//Serial.println(state);
	}
	
	switch(state){
		case MODE_ADDHOUR:{
			if(inp.clickUp())
				clk.addHour();
			if(inp.clickDown())
				clk.addHour(true);
			const RGB	colorParamHeader = { 0, 0,50};
			const RGB 	colorParamValue  = {0,150,50};
			int temp = (int)clk.getRtcHour();
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'H', 5, 6);
			writeMatrix(&matrix, 'R', 5, 2);
			writeNeo(&matrix, &neo, &colorParamHeader, true);
			clearMatrix(&matrix);
			writeMatrix(&matrix,   temp/10, 0, 6);
			writeMatrix(&matrix,   temp%10, 0, 2);
			writeNeo(&matrix, &neo, &colorParamValue);
			break;
		}
		case MODE_ADDMIN:{
			if(inp.clickUp())
				clk.addMinute();
			if(inp.clickDown())
				clk.addMinute(true);
			const RGB	colorParamHeader = { 0, 0,50};
			const RGB 	colorParamValue  = {0,150,50};
			int temp = (int)clk.getRtcMinute();
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'M', 5, 6);
			writeMatrix(&matrix, 'N', 5, 0);
			writeNeo(&matrix, &neo, &colorParamHeader, true);
			clearMatrix(&matrix);
			writeMatrix(&matrix,   temp/10, 0, 6);
			writeMatrix(&matrix,   temp%10, 0, 2);
			writeNeo(&matrix, &neo, &colorParamValue);
			break;
		}
		case MODE_ADDWDAY:{
			if(inp.clickUp())
				clk.addWday();
			if(inp.clickDown())
				clk.addWday(true);
			const RGB	colorParamHeader = { 0, 0,50};
			const RGB 	colorParamValue  = {0,150,50};
			int temp = (int)clk.getRtcWday();
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'W', 5, 4);
			writeMatrix(&matrix, 'D', 5, 0);
			writeNeo(&matrix, &neo, &colorParamHeader, true);
			clearMatrix(&matrix);
			switch(temp){
				case 1:{
					writeMatrix(&matrix,   'M', 0, 5);
					writeMatrix(&matrix,   'O', 0, 1);
					break;
				}
				case 2:{
					writeMatrix(&matrix,   'D', 0, 5);
					writeMatrix(&matrix,   'I', 0, 3);
					break;
				}
				case 3:{
					writeMatrix(&matrix,   'M', 0, 4);
					writeMatrix(&matrix,   'I', 0, 2);
					break;
				}
				case 4:{
					writeMatrix(&matrix,   'D', 0, 6);
					writeMatrix(&matrix,   'O', 0, 2);
					break;
				}
				case 5:{
					writeMatrix(&matrix,   'F', 0, 6);
					writeMatrix(&matrix,   'R', 0, 2);
					break;
				}
				case 6:{
					writeMatrix(&matrix,   'S', 0, 6);
					writeMatrix(&matrix,   'A', 0, 2);
					break;
				}
				case 7:{
					writeMatrix(&matrix,   'S', 0, 6);
					writeMatrix(&matrix,   'O', 0, 2);
					break;
				}
				default:{
					writeMatrix(&matrix,   temp/10, 0, 6);
					writeMatrix(&matrix,   temp%10, 0, 2);
					break;
				}
			}
			writeNeo(&matrix, &neo, &colorParamValue);
			break;
		}
		case MODE_ADDDAY:{
			if(inp.clickUp())
				clk.addDay();
			if(inp.clickDown())
				clk.addDay(true);
			const RGB	colorParamHeader = { 0, 0,50};
			const RGB 	colorParamValue  = {0,150,50};
			int temp = (int)clk.getRtcDay();
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'D', 5, 6);
			writeMatrix(&matrix, 'D', 5, 2);
			writeNeo(&matrix, &neo, &colorParamHeader, true);
			clearMatrix(&matrix);
			writeMatrix(&matrix,   temp/10, 0, 6);
			writeMatrix(&matrix,   temp%10, 0, 2);
			writeNeo(&matrix, &neo, &colorParamValue);
			break;
		}
		case MODE_ADDMON:{
			if(inp.clickUp())
				clk.addMonth();
			if(inp.clickDown())
				clk.addMonth(true);
			const RGB	colorParamHeader = { 0, 0,50};
			const RGB 	colorParamValue  = {0,150,50};
			int temp = (int)clk.getRtcMonth();
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'M', 5, 4);
			writeMatrix(&matrix, 'T', 5, 0);
			writeNeo(&matrix, &neo, &colorParamHeader, true);
			clearMatrix(&matrix);
			writeMatrix(&matrix,   temp/10, 0, 6);
			writeMatrix(&matrix,   temp%10, 0, 2);
			writeNeo(&matrix, &neo, &colorParamValue);
			break;
		}
		case MODE_ADDYEAR:{
			if(inp.clickUp())
				clk.addYear();
			if(inp.clickDown())
				clk.addYear(true);
			const RGB	colorParamHeader = { 0, 0,50};
			const RGB 	colorParamValue  = {0,150,50};
			int temp = (int)clk.getRtcYear();
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'Y', 5, 7);
			writeMatrix(&matrix, 'Y', 5, 3);
			writeNeo(&matrix, &neo, &colorParamHeader, true);
			clearMatrix(&matrix);
			writeMatrix(&matrix,   temp/10, 0, 6);
			writeMatrix(&matrix,   temp%10, 0, 2);
			writeNeo(&matrix, &neo, &colorParamValue);
			break;
		}
		
		
		case MODE_SLEEP:
		case MODE_WAKEUP:{
			const RGB colorOff = {0,0,0};
			clearMatrix(&matrix);
			writeNeo(&matrix, &neo, &colorOff, true);
			if(action == MODE_WAKEUP)
				state = MODE_CLOCK;
			break;
		}
		case MODE_DAILY:{
			if(tmrToclock.ton(true,2000)){
				tmrToclock.ton(false);
				state = MODE_CLOCK;
			}
			const RGB col1 = {0,50,150};
			const RGB col2 = {0,150,50};
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'D', 5, 7);
			writeMatrix(&matrix, 'A', 5, 3);
			writeMatrix(&matrix, 'I', 5, 1);
			writeNeo(&matrix, &neo, &col1, true);
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'L', 0, 6);
			writeMatrix(&matrix, 'Y', 0, 2);
			writeNeo(&matrix, &neo, &col2);
			break;
		}
		case MODE_SCHWEIZ:{
			if(tmrToclock.ton(true,10000)){
				tmrToclock.ton(false);
				state = MODE_CLOCK;
			}
			const RGB white = {50,50,50};
			const RGB red = {50,0,0};
			const struct PATTERN 	View_SchweizWhite =  {
				0b0000000000000,
				 0b00001110000,
				 0b00001110000,
				 0b00001110000,
				 0b01111111110,
				 0b01111111110,
				 0b01111111110,
				 0b00001110000,
				 0b00001110000,
				0b0000011100000};
			const struct PATTERN 	View_SchweizRed =  {
				0b1111111111111,
				 0b11110001111,
				 0b11110001111,
				 0b11110001111,
				 0b10000000001,
				 0b10000000001,
				 0b10000000001,
				 0b11110001111,
				 0b11110001111,
				0b1111100011111};
			writeNeo(&View_SchweizRed, &neo, &red);
			writeNeo(&View_SchweizWhite, &neo, &white);
			break;
		}

		case MODE_BEN:{
			if(tmrToclock.ton(true,10000)){
				tmrToclock.ton(false);
				state = MODE_CLOCK;
			}
			const RGB white = {50,50,50};
			const RGB red = {50,0,0};
			const struct PATTERN 	View_Ben1 =  {
				0b0000000000000, 
				 0b00000000000,
				 0b00000000000,
				 0b00000000000,
				 0b00000000000,
				 0b00001110000,
				 0b00000000000,
				 0b00000000000,
				 0b00000000000,
				0b0000000000000};
			const struct PATTERN 	View_Ben2 =  {
				0b0000000000000,
				 0b00111011100,
				 0b01000100010,
				 0b01000000010,
				 0b01000000010,
				 0b00101110100,
				 0b00010001000,
				 0b00001010000,
				 0b00000100000,
				0b0000000000000};
			const struct PATTERN 	View_Ben3 =  {
				0b0001110111000,
				 0b01000100010,
				 0b10000100001,
				 0b10000000001,
				 0b10000000001,
				 0b01001110010,
				 0b00100000100,
				 0b00010001000,
				 0b00001010000,
				0b0000001000000};
			const struct PATTERN 	View_Ben4 =  {
				0b0001110111000,
				 0b01111111110,
				 0b11000100011,
				 0b11000000011,
				 0b11000000011,
				 0b01101110110,
				 0b00110001100,
				 0b00011011000,
				 0b00001110000,
				0b0000001000000};
			const struct PATTERN 	View_Ben5 =  {
				0b0001110111000,
				 0b01111111110,
				 0b11111111111,
				 0b11100100111,
				 0b11100000111,
				 0b01111111110,
				 0b00111011100,
				 0b00011111000,
				 0b00001110000,
				0b0000001000000};
			const struct PATTERN 	View_Ben6 =  {
				0b0001110111000,
				 0b01111111110,
				 0b11111111111,
				 0b11111111111,
				 0b11111111111,
				 0b01111111110,
				 0b00111111100,
				 0b00011111000,
				 0b00001110000,
				0b0000001000000};
			writeNeo(&View_Ben1, &neo, &red);
			writeNeo(&View_Ben2, &neo, &white);
			break;
		}

		case MODE_AUSTRIA:{
			if(tmrToclock.ton(true,10000)){
				tmrToclock.ton(false);
				state = MODE_CLOCK;
			}
			const RGB white = {50,50,50};
			const RGB red = {50,0,0};
			const struct PATTERN 	View_AustriaWhite =  {
				0b0000000000000,
				 0b00000000000,
				 0b00000000000,
				 0b11111111111,
				 0b11111111111,
				 0b11111111111,
				 0b11111111111,
				 0b00000000000,
				 0b00000000000,
			0b0000000000000};
			const struct PATTERN 	View_AustriaRed =  {
				0b1111111111111,
				 0b11111111111,
				 0b11111111111,
				 0b00000000000,
				 0b00000000000,
				 0b00000000000,
				 0b00000000000,
				 0b11111111111,
				 0b11111111111,
				0b1111111111111};
			writeNeo(&View_AustriaRed, &neo, &red);
			writeNeo(&View_AustriaWhite, &neo, &white);
			break;
		}
		case MODE_INUBIA:{
			if(tmrToclock.ton(true,2000)){
				tmrToclock.ton(false);
				state = MODE_CLOCK;
			}
			const RGB col1 = {40,0,0};
			const RGB col2 = {0,40,0};
			const RGB col3 = {0,0,40};
			const RGB col4 = {30,30,0};
			const RGB col5 = {0,30,30};
			const RGB col6 = {30,0,30};
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'I', 5, 10);
			writeNeo(&matrix, &neo, &col1, true);
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'N', 5, 4);
			writeNeo(&matrix, &neo, &col2);
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'U', 5, 0);
			writeNeo(&matrix, &neo, &col3);
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'B', 0, 7);
			writeNeo(&matrix, &neo, &col4);
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'I', 0, 5);
			writeNeo(&matrix, &neo, &col5);
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'A', 0, 1);
			writeNeo(&matrix, &neo, &col6);
			break;
		}
		
		case MODE_TEMP:{
			if(tmrToclock.ton(true,2000)){
				tmrToclock.ton(false);
				state = MODE_CLOCK;
			}
			const RGB	colorParamHeader = { 0, 0,50};
			int temp = (int)clk.getRtcTemp();
			clearMatrix(&matrix);
			writeMatrix(&matrix, 'T', 5, 7);
			writeMatrix(&matrix, ':', 5, 5);
			writeNeo(&matrix, &neo, &colorParamHeader, true);
			clearMatrix(&matrix);
			writeMatrix(&matrix,   temp/10, 0, 6);
			writeMatrix(&matrix,   temp%10, 0, 2);
			writeMatrix(&matrix, '\'',0, 0);
			writeNeo(&matrix, &neo, &seq.calcColor);
			break;
		}
		case MODE_DCFDIAG:{
			clk.view_stream();
			if(tmrDiagPrintTime.ton(true,10000)){
				tmrDiagPrintTime.ton(false);
				clk.printDateTime();
				// Debug: free Memory
				Serial.print(F("FreeMem:"));
				Serial.println(freeMemory());
			}
			break;
		}
		case MODE_CLOCK:{
			if(inp.clickUp() or inp.clickDown()){
				state = MODE_TEMP;
			}
			generateClockMatrix(&matrix, Hour, Minute);
			writeNeo(&matrix, &neo, &seq.calcColor, true);
			break;
		}
		default:{
			state = MODE_CLOCK;
			break;
		}
	}
	neo.show();
}
