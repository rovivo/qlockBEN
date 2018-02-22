#ifndef EventHandler_h
#define EventHandler_h

#include <Arduino.h>

struct EventType {
	short		year;
	short		month;
	short		day;
	short		wday;
	short		hour;
	short		minute;
	short		second;
	short		mode;
	struct EventType	*next;
};

class EventHandler
{
	public:
		EventHandler();
		void	add(	short mode,
						short year,
						short month,
						short day,
						short hour		= -1,
						short minute	= -1,
						short second	= 0,
						short wday		= -1);
		int		check(	unsigned short year,
						unsigned short month,
						unsigned short day,
						unsigned short hour,
						unsigned short minute,
						unsigned short second,
						unsigned short wday);
		void	print();
		
	private:
		EventType	*eventList	=0;
		EventType	*cursor		=0;
		unsigned short last_year;
		unsigned short last_month;
		unsigned short last_day;
		unsigned short last_hour;
		unsigned short last_minute;
		unsigned short last_second;
		unsigned short last_wday;
};


#endif