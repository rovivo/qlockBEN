#include "EventHandler.h"

EventHandler::EventHandler(){
}

void EventHandler::add(	short mode,
						short year,
						short month,
						short day,
						short hour,
						short minute,
						short second,
						short wday
						){
	if(cursor = new(EventType)){
		cursor->mode	= mode;
		cursor->year	= year;
		cursor->month	= month;
		cursor->day		= day;
		cursor->hour	= hour;
		cursor->minute	= minute;
		cursor->second	= second;
		cursor->wday	= wday;
		cursor->next	= eventList;
		eventList		= cursor;
	}
	else{
		delay(3000);
		Serial.println("ERROR (allocation failed)");
	}
}

int EventHandler::check(unsigned short year,
						unsigned short month,
						unsigned short day,
						unsigned short hour,
						unsigned short minute,
						unsigned short second,
						unsigned short wday){	
	if(second != last_second){
		last_year		= year;
		last_month		= month;
		last_day		= day;
		last_hour		= hour;
		last_minute		= minute;
		last_second		= second;
		cursor = eventList;
		while(cursor){
			
			if(
					(cursor->year == (short)year or cursor->year == -1)
				and	(cursor->month == (short)month or cursor->month == -1)
				and	(cursor->day == (short)day or cursor->day == -1)
				and	(cursor->hour == (short)hour or cursor->hour == -1)
				and	(cursor->minute	== (short)minute or cursor->minute == -1)
				and	(cursor->second == (short)second or second == -1)
				and	(cursor->wday == (short)wday or cursor->wday == -1)
			){
				//Serial.print("Event detected: ");
				//Serial.println(cursor->mode);
				return cursor->mode;
			}
			cursor = cursor->next;
		}
	}
	return 0;
}

void EventHandler::print(){
	//Serial.println("Event-List:");
	//cursor	= eventList;
	//int num	= 0;
	//while(cursor){
	//	num++;
	//	Serial.print(num);
	//	Serial.print(" Act: ");
	//	Serial.print(cursor->mode);
	//	Serial.print(", Y:");
	//	Serial.print(cursor->year);
	//	Serial.print(", M:");
	//	Serial.print(cursor->month);
	//	Serial.print(", D:");
	//	Serial.print(cursor->day);
	//	Serial.print(", H:");
	//	Serial.print(cursor->hour);
	//	Serial.print(", I:");
	//	Serial.print(cursor->minute);
	//	Serial.print(", S:");
	//	Serial.print(cursor->second);
	//	Serial.print(", W:");
	//	Serial.print(cursor->wday);
	//	Serial.println();
	//	cursor	= cursor->next;
	//}
}