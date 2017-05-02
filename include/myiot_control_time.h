/*
 * myiot_control_time.h
 *
 *  Created on: 22.04.2017
 *      Author: a4711
 */

#ifndef MYIOT_CONTROL_TIME_H_
#define MYIOT_CONTROL_TIME_H_

#include "arduino_mock.h"

namespace MyIOT
{
    /// Simple control on/off based on time of day.
    /* With this control you can define an interval within one day.
     * If you pass a time (HourMinuteSecond) to the control,
     * it will tell you if the given time is within the interval.
     * */
	class ControlTime
	{
	public:
		class HourMinuteSecond
		{
		public:
			HourMinuteSecond(int hour, int minute = 0, int second = 0) :
					m_hour(hour), m_minute(minute), m_second(second)
			{
			}
			HourMinuteSecond(const HourMinuteSecond& src) :
					m_hour(src.m_hour), m_minute(src.m_minute), m_second(
							src.m_second)
			{
			}
			const HourMinuteSecond&
			operator=(const HourMinuteSecond& src)
			{
				m_hour = src.m_hour;
				m_minute = src.m_minute;
				m_second = src.m_second;
				return *this;
			}
			bool operator <(const HourMinuteSecond& other) const
			{
				if (m_hour < other.m_hour)
					return true;
				if (m_hour == other.m_hour)
				{
					if (m_minute < other.m_minute)
						return true;
					if (m_minute == other.m_minute && m_second < other.m_second)
						return true;
				}
				return false;
			}
			bool operator ==(const HourMinuteSecond& other) const
			{
				return m_hour == other.m_hour && m_minute == other.m_minute
						&& m_second == other.m_second;
			}
			bool operator <=(const HourMinuteSecond& other) const
			{
				return *this < other || *this == other;
			}

			int hour()
			{
				return m_hour;
			}
			int minute()
			{
				return m_minute;
			}
		private:
			int m_hour;
			int m_minute;
			int m_second;
		};

		ControlTime(const HourMinuteSecond& begin, const HourMinuteSecond& end) :
				m_begin(begin), m_end(end)
		{
		}

		bool check(const HourMinuteSecond& current) const
		{
			if (m_begin < m_end)
			{
				return (m_begin <= current && current <= m_end);
			}
			return !(m_end < current && current < m_begin);
		}

		bool check() const
		{
			time_t tnow;
			time(&tnow);
			tm* loc = localtime(&tnow);
			if (loc)
			{
				return check(HourMinuteSecond(loc->tm_hour, loc->tm_min));
			}
			else
			{
				Serial.println("ControlTime::check : localtime failed");
			}
			return false;
		}

	private:
		HourMinuteSecond m_begin;
		HourMinuteSecond m_end;
	};
	// class ControlTime

}

#endif /* MYIOT_CONTROL_TIME_H_ */
