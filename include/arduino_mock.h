#pragma once

#include <chrono>
#include <iostream>

class Serial_C
{
public:
	void print(int i)
	{
		std::cout << i;
	}
	void println(int i)
	{
		std::cout << i << std::endl;
	}
	void print(long i)
	{
		std::cout << i;
	}
	void println(long i)
	{
		std::cout << i << std::endl;
	}
	void print(unsigned long i)
	{
		std::cout << i;
	}
	void println(unsigned long i)
	{
		std::cout << i << std::endl;
	}
	void print(const char* tx)
	{
		std::cout << tx;
	}
	void println(const char* tx)
	{
		std::cout << tx << std::endl;
	}
};
static Serial_C Serial;

inline void delay(int milliseconds)
{
	timespec req;
	timespec rem;
	req.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
	req.tv_sec = (milliseconds / 1000);
	rem.tv_nsec = 0;
	rem.tv_sec = 0;
	nanosleep(&req, &rem);
}

inline unsigned long millis()
{
    using namespace std::chrono;

    auto epoch = high_resolution_clock::from_time_t(0);
    auto now   = high_resolution_clock::now();
    auto mseconds = duration_cast<milliseconds>(now - epoch).count();
    return mseconds;
}
