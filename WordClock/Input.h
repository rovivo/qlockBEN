#ifndef Input_h
#define Input_h

class Input
{
	public:
		Input(short _pinBtnUp, short _pinBtnMode, short _pinBtnDown);
		void setup();
		void call();
		bool clickUp(bool fn=false);
		bool clickMode(bool fn=false);
		bool clickDown(bool fn=false);
		bool buttonUp();
		bool buttonMode();
		bool buttonDown();
		bool doReset();
		
	private:
		short	pin_Up;
		short	pin_Mode;
		short	pin_Down;
		
		bool	actUp;
		bool	actMode;
		bool	actDown;
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