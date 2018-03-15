#ifndef MyClock_h
#define MyClock_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define		MYCLOCK_STREAMLEN				14		// Max length of bit-stream in bytes


class MyClock
{
	public:
		MyClock(short _pinDCF, short _pinRTCEnable, Adafruit_NeoPixel* _neo);
		void  			setup();
		void  			call();
						
		void  			stream_add(bool _bit);
		void  			stream_end();
		//void  			stream_print();
		void  			stream_check();
		void  			view_stream();
						
		bool  			timeValid();
		unsigned short  getDcfHour();
		unsigned short  getDcfMinute();
		unsigned short  getDcfDay();
		unsigned short  getDcfWday();
		unsigned short  getDcfMonth();
		unsigned short  getDcfYear();
		void			printDateTime();
		
		unsigned short	hex2bcd(unsigned short in);
		unsigned short	bcd2hex(unsigned short in);
		unsigned short  getRtcHour();
		unsigned short  getRtcMinute();
		unsigned short  getRtcSecond();
		unsigned short  getRtcDay();
		unsigned short  getRtcWday();
		unsigned short  getRtcMonth();
		unsigned short  getRtcYear();
		float			getRtcTemp();
		void			resetRtc();
		
		void			adjustRTCClock();
		void  			addHour(bool inv=false);
		void  			addMinute(bool inv=false);
		void  			addDay(bool inv=false);
		void  			addWday(bool inv=false);
		void  			addMonth(bool inv=false);
		void  			addYear(bool inv=false);
		
		
	private:
		Adafruit_NeoPixel* 		neo;
		
		short			pinRTCEnable;
		short			pinDCF;
		bool			pinState;
		bool			pinStateOld;
		
		unsigned long	lastPos;
		unsigned long	lastNeg;
		
		bool			bitstream_invalid;
		short			bitstream[MYCLOCK_STREAMLEN];
		short			bitstream_lastValid[MYCLOCK_STREAMLEN];
		short			bitstream_idx;
		
		bool			validDCF;
		bool			initRTC;
		unsigned short	dcfHour;
		unsigned short	dcfMinute;
		unsigned short	dcfDay;
		unsigned short	dcfWday;
		unsigned short	dcfMonth;
		unsigned short	dcfYear;
		
		unsigned short	rtcHour;
		unsigned short	rtcMinute;
		unsigned short	rtcSecond;
		unsigned short	rtcDay;
		unsigned short	rtcWday;
		unsigned short	rtcMonth;
		unsigned short	rtcYear;
		float			rtcTemp;

};

#endif