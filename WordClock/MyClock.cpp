#include "MyClock.h"

MyClock::MyClock(short _pinDCF, short _pinRTCEnable, Adafruit_NeoPixel* _neo){
	pinDCF			= _pinDCF;
	pinRTCEnable	= _pinRTCEnable;
	neo = _neo;
}

void MyClock::setup(){
	// Configuration
	pinMode(pinRTCEnable, OUTPUT);
	digitalWrite(pinRTCEnable, HIGH);
	pinMode(pinDCF, INPUT_PULLUP);
	
	Wire.begin();
	

	// Init pinState
	pinState		= false;
	pinStateOld		= false;
	lastPos			= 0;
	lastNeg			= 0;
	
	validDCF		= false;
	dcfHour			= 0;
	dcfMinute		= 0;
	dcfDay			= 0;
	dcfMonth		= 0;
	dcfYear			= 0;
	
	// Init Bitstream
	bitstream_invalid = true;
	stream_end();
}

void MyClock::call(){
	// Update current time from RTC
	Wire.beginTransmission(0x68);
	Wire.write(0);
	Wire.endTransmission();
	Wire.requestFrom(0x68, 7);
	rtcSecond	= bcd2hex(Wire.read());
	rtcMinute	= bcd2hex(Wire.read());
	rtcHour		= bcd2hex(Wire.read());
	rtcWday		= bcd2hex(Wire.read());
	rtcDay		= bcd2hex(Wire.read());
	rtcMonth	= bcd2hex(Wire.read());
	rtcYear		= bcd2hex(Wire.read());
	Wire.beginTransmission(0x68);
	Wire.write(0x11);
	Wire.endTransmission();
	Wire.requestFrom(0x68, 2);
	int msb		= Wire.read();
	int lsb		= Wire.read();
	rtcTemp		= ((msb << 2) + (lsb >> 6) ) / 4.0;

	// Read Input & aux signals (DCF)
	pinState = !digitalRead(pinDCF);
	bool	sigPos	= pinState and !pinStateOld;
	bool	sigNeg	= !pinState and pinStateOld;
	pinStateOld		= pinState;
	
	// Signal detection
	if(sigPos){
		lastNeg = millis();
		unsigned long timeFalse = lastNeg-lastPos;
		//	Serial.print(" Sig-: ");
		//	Serial.println(timeFalse);
		if(timeFalse < 450){				// Pause was too short
			bitstream_invalid = true;
			Serial.print(" ! Sig- too short: ");
			Serial.println(timeFalse);
		}
		else if(timeFalse > 800)
			stream_end();
		else if(timeFalse > 1600){ 		// Pause was too long
			bitstream_invalid = true;
			Serial.print(" ! Sig- too long: ");
			Serial.println(timeFalse);
		}
	}
	if(sigNeg){
		lastPos = millis();
		unsigned long timeTrue = lastPos-lastNeg;
		//	Serial.print("               Sig+: ");
		//	Serial.println(timeTrue);
		if(timeTrue < 50){
			bitstream_invalid = true;
			stream_add(0);
			Serial.print(" ! Sig+ too short: ");
			Serial.println(timeTrue);
		}
		else if(timeTrue < 95){
			stream_add(0);
		}
		else if(timeTrue < 105){
			bitstream_invalid = true;
			stream_add(1);
			Serial.print(" ! Sig undef.: ");
			Serial.println(timeTrue);
		}
		else if(timeTrue < 150){
			stream_add(1);
		}
		else{
			bitstream_invalid = true;
			stream_add(0);
			Serial.print(" ! Sig+ too long: ");
			Serial.println(timeTrue);
		}
	}
	
	// Adjust RTC
	if(validDCF or initRTC){
		validDCF = false;
		if(initRTC){
			initRTC = false;
			dcfMinute	= 1;
			dcfHour		= 1;
			dcfWday		= 1;
			dcfDay		= 1;
			dcfMonth	= 1;
			dcfYear		= 1;
		}
		Wire.beginTransmission(0x68);
		Wire.write(0);
		Wire.write(1);// Seconds always to 01s
		Wire.write(hex2bcd(dcfMinute));
		Wire.write(hex2bcd(dcfHour));
		Wire.write(hex2bcd(dcfWday));
		Wire.write(hex2bcd(dcfDay));
		Wire.write(hex2bcd(dcfMonth));
		Wire.write(hex2bcd(dcfYear));
		Wire.endTransmission();
	}
}

void MyClock::stream_add(bool bit){
	short arrNum = bitstream_idx/8;
	short arrIdx = bitstream_idx%8;
	if(bit)
		bitstream[arrNum] += (1 << arrIdx);
	bitstream_idx++;
	if(bitstream_idx >= MYCLOCK_STREAMLEN * 8)
		bitstream_idx = 0;
}

void MyClock::stream_end(){
	neo->setPixelColor(112, 100,0,0);
	neo->setPixelColor(113, 100,0,0);
	if(!bitstream_invalid)
		stream_check();
	else
		Serial.println("Stream invalid");

	bitstream_invalid = false;
	bitstream_idx = 0;
	for(int i=0; i<MYCLOCK_STREAMLEN; i++)
		bitstream[i] = 0;
}

void MyClock::view_stream(){
	short pixel = 0;
	for(int n=0; n<MYCLOCK_STREAMLEN; n++){
		for(int i=0; i<8; i++){
			if(bitstream_idx > (n*8 + i)){
				if(bitstream[n] & (1 <<  i))
					neo->setPixelColor(pixel++, 0,50,0);
				else
					neo->setPixelColor(pixel++, 50,0,0);
			}
			else
				neo->setPixelColor(pixel++, 0,0,0);
		}
	}
	neo->show();
}

//void MyClock::stream_print(){
//		Serial.println(": Stream Print :: ");
//		for(int n=0; n<MYCLOCK_STREAMLEN; n++){
//			for(int i=0; i<8; i++){
//				if(bitstream[n] & (1 <<  i))
//					Serial.print("1");
//				else
//					Serial.print("0");
//			}
//		}
//		Serial.println();
//}

void MyClock::stream_check(){
	short _min		= 0;
	short _minP		= false; // Parity-counter minute
	short _hour		= 0;
	short _hourP	= false; // Parity-counter hour
	short _day		= 0;
	short _wday		= 0;
	short _month	= 0;
	short _year		= 0;
	short _dateP	= false; // Parity-counter date
	
	if(bitstream_idx != 59){
		Serial.println("Fail: Length");
		return;
	}
	
	for(int n=0; n<60; n++){
		short idx = n/8;
		short bit = n%8;
		bool b = (bitstream[idx] & (1 <<  bit));
		
		switch(n){
			case 0:
				if(b){
					Serial.println("Fail: First bit");
					return;
				}
				break;
			case 20:
				if(!b){
					Serial.println("Fail: Bit 20");
					return;
				}
				break;
			case 21:
				if(b){
					_minP++;
					_min += 1;
				}
				break;
			case 22:
				if(b){
					_minP++;
					_min += 2;
				}
				break;
			case 23:
				if(b){
					_minP++;
					_min += 4;
				}
				break;
			case 24:
				if(b){
					_minP++;
					_min += 8;
				}
				break;
			case 25:
				if(b){
					_minP++;
					_min += 10;
				}
				break;
			case 26:
				if(b){
					_minP++;
					_min += 20;
				}
				break;
			case 27:
				if(b){
					_minP++;
					_min += 40;
				}
				break;
			case 28:
				if(b){
					_minP++;
				}
				if(_minP%2 == 1){
					Serial.println("Failed: Parity min");
					return;
				}
				if(_min < 1 or _min > 59){
					Serial.print("Fail: min ");
					Serial.println(_min);
					return;
				}
				dcfMinute = _min;
				break;
			case 29:
				if(b){
					_hourP++;
					_hour += 1;
				}
				break;
			case 30:
				if(b){
					_hourP++;
					_hour += 2;
				}
				break;
			case 31:
				if(b){
					_hourP++;
					_hour += 4;
				}
				break;
			case 32:
				if(b){
					_hourP++;
					_hour += 8;
				}
				break;
			case 33:
				if(b){
					_hourP++;
					_hour += 10;
				}
				break;
			case 34:
				if(b){
					_hourP++;
					_hour += 20;
				}
				break;
			case 35:
				if(b){
					_hourP++;
				}
				if(_hourP%2 == 1){
					Serial.println("Fail: Parity hour");
					return;
				}
				if(_hour < 1 or _hour > 23){
					Serial.print("Fail: Hour ");
					Serial.println(_hour);
					return;
				}
				dcfHour = _hour;
				break;
			case 36:
				if(b){
					_dateP++;
					_day += 1;
				}
				break;
			case 37:
				if(b){
					_dateP++;
					_day += 2;
				}
				break;
			case 38:
				if(b){
					_dateP++;
					_day += 4;
				}
				break;
			case 39:
				if(b){
					_dateP++;
					_day += 8;
				}
				break;
			case 40:
				if(b){
					_dateP++;
					_day += 10;
				}
				break;
			case 41:
				if(b){
					_dateP++;
					_day += 20;
				}
				break;
			case 42:
				if(b){
					_dateP++;
					_wday += 1;
				}
				break;
			case 43:
				if(b){
					_dateP++;
					_wday += 2;
				}
				break;
			case 44:
				if(b){
					_dateP++;
					_wday += 4;
				}
				break;
			case 45:
				if(b){
					_dateP++;
					_month += 1;
				}
				break;
			case 46:
				if(b){
					_dateP++;
					_month += 2;
				}
				break;
			case 47:
				if(b){
					_dateP++;
					_month += 4;
				}
				break;
			case 48:
				if(b){
					_dateP++;
					_month += 8;
				}
				break;
			case 49:
				if(b){
					_dateP++;
					_month += 10;
				}
				break;
			case 50:
				if(b){
					_dateP++;
					_year += 1;
				}
				break;
			case 51:
				if(b){
					_dateP++;
					_year += 2;
				}
				break;
			case 52:
				if(b){
					_dateP++;
					_year += 4;
				}
				break;
			case 53:
				if(b){
					_dateP++;
					_year += 8;
				}
				break;
			case 54:
				if(b){
					_dateP++;
					_year += 10;
				}
				break;
			case 55:
				if(b){
					_dateP++;
					_year += 20;
				}
				break;
			case 56:
				if(b){
					_dateP++;
					_year += 40;
				}
				break;
			case 57:
				if(b){
					_dateP++;
					_year += 80;
				}
				break;
			case 58:
				if(b){
					_dateP++;
					if(_hourP%2 == 1){
							Serial.println("Fail: Parity date");
						return;
					}
					if(_day < 1 or _day > 31){
							Serial.println("Fail: Day");
						return;
					}
					if(_month < 1 or _month > 12){
							Serial.println("Fail: Month");
						return;
					}
					if(_year < 18 or _year > 30){
							Serial.println("Fail: Year");
						return;
					}
					dcfDay		= _day;
					dcfWday		= _wday;
					dcfMonth	= _month;
					dcfYear		= _year;
					neo->setPixelColor(112, 0,100,0);
					neo->setPixelColor(113, 0,100,0);
				}
				break;
			default:
				break;
		}
	}
	Serial.println();
	Serial.println("DT valid ");
	Serial.print(dcfHour);
	Serial.print(":");
	Serial.print(dcfMinute);
	Serial.print(" ");
	Serial.print(dcfDay);
	Serial.print(".");
	Serial.print(dcfMonth);
	Serial.print(".");
	Serial.println(dcfYear);
	validDCF = true;
}

bool MyClock::timeValid(){
	bool out = validDCF;
	validDCF = false;
	return out;
}

unsigned short MyClock::getDcfHour(){
	return dcfHour;
}

unsigned short MyClock::getDcfMinute(){
	return dcfMinute;
}

unsigned short MyClock::getDcfDay(){
	return dcfDay;
}

unsigned short MyClock::getDcfWday(){
	return dcfWday;
}

unsigned short MyClock::getDcfMonth(){
	return dcfMonth;
}

unsigned short MyClock::getDcfYear(){
	return dcfYear;
}

void MyClock::printDateTime(){
	Serial.print("DT: ");
	Serial.print(rtcHour, DEC);
	Serial.print(":");
	Serial.print(rtcMinute, DEC);
	Serial.print(":");
	Serial.print(rtcSecond, DEC);
	Serial.print(" ");
	Serial.print(rtcDay, DEC);
	Serial.print(".");
	Serial.print(rtcMonth, DEC);
	Serial.print(".");
	Serial.print(rtcYear, DEC);
	Serial.print(" (");
	Serial.print(rtcWday, DEC);
	Serial.println(")");
}

unsigned short MyClock::hex2bcd(unsigned short in){
	return in + 6 * (in / 10);
}

unsigned short MyClock::bcd2hex(unsigned short in){
	return in - 6 * (in >> 4);
}

unsigned short  MyClock::getRtcHour(){
	return rtcHour;
}

unsigned short  MyClock::getRtcMinute(){
	return rtcMinute;
}

unsigned short  MyClock::getRtcSecond(){
	return rtcSecond;
}

unsigned short  MyClock::getRtcDay(){
	return rtcDay;
}

unsigned short  MyClock::getRtcWday(){
	return rtcWday;
}

unsigned short  MyClock::getRtcMonth(){
	return rtcMonth;
}

unsigned short  MyClock::getRtcYear(){
	return rtcYear;
}

float  MyClock::getRtcTemp(){
	return rtcTemp;
}

void MyClock::resetRtc(){
	initRTC = true;
}

void MyClock::adjustRTCClock(){
	Wire.beginTransmission(0x68);
	Wire.write(0);
	Wire.write(1);// Seconds always to 01s
	Wire.write(hex2bcd(rtcMinute));
	Wire.write(hex2bcd(rtcHour));
	Wire.write(hex2bcd(rtcWday));
	Wire.write(hex2bcd(rtcDay));
	Wire.write(hex2bcd(rtcMonth));
	Wire.write(hex2bcd(rtcYear));
	Wire.endTransmission();
}
void MyClock::addHour(bool inv){
	if(!inv and rtcHour < 23){
		rtcHour++;
		adjustRTCClock();
	}
	if(inv and rtcHour > 0){
		rtcHour--;
		adjustRTCClock();
	}
}
void MyClock::addMinute(bool inv){
	if(!inv and rtcMinute < 59){
		rtcMinute++;
		adjustRTCClock();
	}
	if(inv and rtcMinute > 0){
		rtcMinute--;
		adjustRTCClock();
	}
}
void MyClock::addDay(bool inv){
	if(!inv and rtcDay < 31){
		rtcDay++;
		adjustRTCClock();
	}
	if(inv and rtcDay > 1){
		rtcDay--;
		adjustRTCClock();
	}
}
void MyClock::addWday(bool inv){
	if(!inv and rtcWday < 7){
		rtcWday++;
		adjustRTCClock();
	}
	if(inv and rtcWday > 1){
		rtcWday--;
		adjustRTCClock();
	}
}
void MyClock::addMonth(bool inv){
	if(!inv and rtcMonth < 12){
		rtcMonth++;
		adjustRTCClock();
	}
	if(inv and rtcMonth > 1){
		rtcMonth--;
		adjustRTCClock();
	}
}
void MyClock::addYear(bool inv){
	if(!inv and rtcYear < 40){
		rtcYear++;
		adjustRTCClock();
	}
	if(inv and rtcYear > 0){
		rtcYear--;
		adjustRTCClock();
	}
}
