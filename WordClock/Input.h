#ifndef Input_h
#define Input_h

class Input
{
	public:
		Input(short _pinBtnUp, short _pinBtnMode, short _pinBtnDown, int _pinLDR);
		void setup();
		void call();
		bool clickUp(bool fn=false);
		bool clickMode(bool fn=false);
		bool clickDown(bool fn=false);
		int actBrightness();
		bool buttonUp();
		bool buttonMode();
		bool buttonDown();
		bool doReset();
		
	private:
		short	pin_Up;
		short	pin_Mode;
		short	pin_Down;
		int 	pin_LDR;
		
		bool	actUp;
		bool	actMode;
		bool	actDown;
		int 	actLDR;
		bool	oldUp;
		bool	oldMode;
		bool	oldDown;
		
		bool	fpUp;
		bool	fpMode;
		bool	fpDown;
		bool	fnUp;
		bool	fnMode;
		bool	fnDown;
};

#endif