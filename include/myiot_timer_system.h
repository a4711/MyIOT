/*
 * myiot_timer_system.h
 *
 *  Created on: 22.04.2017
 *      Author: a4711
 */

#ifndef MYIOT_TIMER_SYSTEM_H_
#define MYIOT_TIMER_SYSTEM_H_

#include <stdint.h>

#include "arduino_mock.h"

namespace MyIOT
{
class ITimer
{
public:
	virtual ~ITimer()
	{
	}
	/// when it is time to execute the task, "expire" will be called.
	virtual void expire() = 0;

	/// When time "TimerSystem" is destroyed, it will call "destroy" on every task.
	/* Here you can place cleanup code, e.g. "delete this".
	 * */
	virtual void destroy() = 0;
};


/// The timer system allows the schedule several time based tasks, without consuming processing time.
/* To define a new task, derive your task from interface "ITimer" and implement "expire" and "destroy".
 * The task will be scheduled with a "TimeSpec", i.e. the repeating interval of the task.
 * */
class TimerSystem
{
public:
	class TimeSpec
	{
		static const uint64_t MAX_NS = 1000ull * 1000ull * 1000ull;

	public:
		TimeSpec(uint64_t seconds = 0, uint64_t nanoseconds = 0)
		{
			tv_sec = seconds;
			tv_nsec = nanoseconds;
		}
		TimeSpec(const TimeSpec& src)
		{
			tv_sec = src.sec();
			tv_nsec = src.nsec();
		}
		const TimeSpec& operator=(const TimeSpec& src)
		{
			tv_sec = src.sec();
			tv_nsec = src.nsec();
			return *this;
		}

		const TimeSpec& operator+=(const TimeSpec& val)
		{
			tv_sec = this->sec() + val.sec() + (this->nsec() + val.nsec()) / MAX_NS;
			tv_nsec = (this->nsec() + val.nsec() ) % MAX_NS;
			return *this;
		}

		void add_milliseconds(unsigned long milliseconds)
		{
			uint64_t ns = milliseconds * 1000ull * 1000ull;
			tv_sec = this->sec() + (this->nsec() + ns) / MAX_NS;
			tv_nsec = (this->nsec() + ns) % MAX_NS;
		}

		bool operator <(const TimeSpec right) const
		{
			if (this->tv_sec < right.tv_sec)
				return true;
			if (this->tv_sec == right.tv_sec
					&& this->tv_nsec < right.tv_nsec)
				return true;
			return false;
		}

		bool operator >(const TimeSpec right) const
		{
			if (this->tv_sec > right.tv_sec)
				return true;
			if (this->tv_sec == right.tv_sec
					&& this->tv_nsec > right.tv_nsec)
				return true;
			return false;
		}
		bool operator >=(const TimeSpec right) const
		{
			return (*this > right || *this == right);
		}
		bool operator <=(const TimeSpec right) const
		{
			return (*this < right || *this == right);
		}
		bool operator ==(const TimeSpec right) const
		{
			if (this->tv_sec == right.tv_sec
					&& this->tv_nsec == right.tv_nsec)
				return true;
			return false;
		}
		bool operator !=(const TimeSpec right) const
		{
			return !(*this == right);
		}

		uint64_t sec() const
		{
			return tv_sec;
		}
		uint64_t nsec() const
		{
			return tv_nsec;
		}

		void dump()
		{
			Serial.print("::");
			Serial.print((unsigned long)sec());
			Serial.print(":");
			Serial.println((unsigned long)nsec());
		}

	private:
		uint64_t tv_sec;
		uint64_t tv_nsec;
	};

	TimerSystem(): head(nullptr), last_wakeup(millis())
	{
	}

	~TimerSystem()
	{
		reset();
	}

	void reset()
	{
		while (nullptr != head)
		{
			remove(head->get_timer());
		}
	}

	class CallbackTimer: public ITimer
	{
	public:

		CallbackTimer( void(*f)() = nullptr):expire_counter(0)
	    {
			callback = f;
	    }

		bool is_expired() const {return 0 != expire_counter;}
		void reset(){expire_counter = 0;}
		unsigned long get_expire_counter() const {return expire_counter;}

		void expire() override
		{
			expire_counter++;
			if (nullptr != callback)
				callback();
		}
		void destroy() override
		{
			delete this;
		}
	private:
		void (*callback)();
		unsigned long expire_counter;
	};


	size_t count() const
	{
		size_t ret = 0;
		for (Node* node = head; nullptr != node; node = node->get_next())
		{
			ret++;
		}
		return ret;
	}

	bool add(ITimer* timer, const TimeSpec& tspec)
	{
		if (nullptr == timer)
			return false;
		Node* node = new Node(*timer, tspec);
		if (nullptr == node)
			return false;
		if (nullptr == head)
		{
			head = node;
			return true;
		}
		return head->append(node);
	}

	bool remove(const ITimer& timer)
	{
		Node* predecessor = nullptr;
		for (Node* node = head; nullptr != node; node = node->get_next())
		{
			if (node->is_equal(timer))
			{
				if (nullptr != predecessor)
				{
					predecessor->set_next(node->get_next());
				}
				else
				{
					// no predecessor is head !
					head = node->get_next();
				}
				delete node;
				return true;
			}
			else
			{
				predecessor = node;
			}
		}
		return false;
	}

	void expire(const TimeSpec& now)
	{
		for (Node* node = head; nullptr != node; node = node->get_next())
		{
			if (node->should_expire(now))
			{
				node->expire();
				node->calc_next_expiration(now);
			}
		}
	}

	void run_loop(int tick_in_milliseconds, int repeat = -1 /*-1 is forever*/)
	{
		for (int i = 0; repeat == -1 || i < repeat; i++ )
		{
			unsigned long curval = millis();
			unsigned long addval = curval - last_wakeup;
			last_wakeup = curval;
			this->current.add_milliseconds(addval);
			expire(this->current);
			delay(tick_in_milliseconds);
		}
	}

private:
	class Node
	{
	public:
		Node(ITimer& xtimer, const TimeSpec& xtspec) :
				next(nullptr), timer(xtimer), tspec(xtspec)
		{
		}
		~Node()
		{
			timer.destroy();
		}

		Node*
		get_next() const
		{
			return next;
		}
		ITimer&
		get_timer()
		{
			return timer;
		}

		bool append(Node* node)
		{
			if (nullptr != next)
				return next->append(node);
			next = node;
			return true;
		}

		bool is_equal(const ITimer& other)
		{
			return &timer == &other;
		}

		void set_next(Node* xnext)
		{
			next = xnext;
		}

		bool should_expire(const TimeSpec& now)
		{
			return (now >= next_expiration);
		}

		void expire()
		{
			timer.expire();
		}

		void calc_next_expiration(const TimeSpec& now)
		{
			while (now >= next_expiration)
			{
				next_expiration += tspec;
			}
		}

	private:
		Node* next;
		ITimer& timer;
		TimeSpec tspec;
		TimeSpec next_expiration;
	};

	// ------------------------
	Node * head;
	TimeSpec current;
	unsigned long last_wakeup;
};

} // namespace MyIOT

#endif /* MYIOT_TIMER_SYSTEM_H_ */
