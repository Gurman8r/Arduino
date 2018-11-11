/* TimePoint.h
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
* * * * * * * * * * * * * * * */
#ifndef _TIME_POINT_H_
#define _TIME_POINT_H_

#include <inttypes.h>

class TimePoint final
{
private:
	uint64_t m_millis;
	
public:
	TimePoint()
		: m_millis(0) {
	}
	TimePoint(uint64_t value)
		: m_millis(value) {
	}
	TimePoint(const TimePoint & copy)
		: m_millis(copy.m_millis) {
	}
	~TimePoint() {
	}

	inline const uint64_t millis() const {
		return m_millis;
	}
	inline const uint64_t seconds() const {
		return millis() / 1000;
	}
	inline const uint64_t minutes() const {
		return seconds() / 60;
	}
	inline const uint64_t hours() const {
		return minutes() / 24;
	}

	inline friend TimePoint	operator+(const TimePoint & lhs, const TimePoint & rhs) {
		return (lhs + rhs.millis());
	}
	inline friend TimePoint	operator-(const TimePoint & lhs, const TimePoint & rhs) {
		return (lhs - rhs.millis());
	}
	inline friend TimePoint & operator+=(TimePoint & lhs, const TimePoint & rhs) {
		return (lhs = (lhs + rhs));
	}
	inline friend TimePoint & operator-=(TimePoint & lhs, const TimePoint & rhs) {
		return (lhs = (lhs - rhs));
	}

	inline friend TimePoint	operator+(const TimePoint & lhs, uint64_t rhs) {
		return TimePoint(lhs.millis() + rhs);
	}
	inline friend TimePoint	operator-(const TimePoint & lhs, uint64_t rhs) {
		return TimePoint(lhs.millis() - rhs);
	}
	inline friend TimePoint &	operator+=(TimePoint & lhs, uint64_t rhs) {
		return (lhs = (lhs + rhs));
	}
	inline friend TimePoint &	operator-=(TimePoint & lhs, uint64_t rhs) {
		return (lhs = (lhs - rhs));
	}
};

#endif