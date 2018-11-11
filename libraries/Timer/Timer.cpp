/* Timer.cpp
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
* * * * * * * * * * * * * * * */
#include <Timer.h>

#if defined(ARDUINO)
	#include <Arduino.h>
	#define DELTA_TIME millis
#endif


Timer::Timer()
	: m_paused(true) 
	, m_elapsed(0)
	, m_max(0)
	, m_min(0) {
}
Timer::Timer(const Timer & copy)
	: m_paused(copy.m_paused)
	, m_elapsed(copy.m_elapsed) 
	, m_max(copy.m_max)
	, m_min(copy.m_min) {
}
Timer::~Timer() {
}


Timer& Timer::start() {
	m_min = m_max = DELTA_TIME();
	m_elapsed = 0;
	return pause(false);
}

Timer& Timer::reset() {
	return stop().start();
}

Timer& Timer::stop() {
	m_elapsed = ((m_max = DELTA_TIME()) - m_min);
	return (*this);
}

Timer& Timer::pause(bool pause) {
	if (pause && !m_paused)
	{
		m_paused = true;
		m_elapsed = ((m_max = DELTA_TIME()) - m_min);
	}
	else if (!pause && m_paused)
	{
		m_paused = false;
		m_min = DELTA_TIME();
	}
	return (*this);
}

const TimePoint & Timer::elapsed() const {
	if(!m_paused) {
		m_elapsed = (DELTA_TIME() - m_min.millis());
	}
	return m_elapsed;
}