/*
*/
#ifndef _TIMER_H_
#define _TIMER_H_

#include <TimePoint.h>

class Timer final
{
public:
	Timer();
	Timer(const Timer& copy);
	~Timer();

	Timer& pause(bool pause);
	Timer& reset();
	Timer& start();
	Timer& stop();

	const TimePoint & elapsed() const;
	
	inline const bool & paused() const {
		return m_paused;
	}

private:
	bool m_paused;
	
	mutable TimePoint m_elapsed, m_max, m_min;
};


#endif // !_TIMER_H_