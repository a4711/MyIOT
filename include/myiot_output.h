/*
 * myiot_ssr.h
 *
 *  Created on: 22.04.2017
 *      Author: a4711
 */

#ifndef MYIOT_OUTPUT_H_
#define MYIOT_OUTPUT_H_

#include "mraa.hpp"


namespace MyIOT
{
	class Output
	{
	public:
		Output(int pin) :
				pin_number(pin), pin(0)
		{
			setup();
		}

		void setup()
		{
			pin = new mraa::Gpio(pin_number, true, false);
			pin->dir(mraa::DIR_OUT);
			set(false);
			//pinMode(pin_number, OUTPUT);
		}

		operator bool () const
	    {
			return this->get();
	    }

		const Output& operator=(bool enable)
		{
			set(enable);
			return *this;
		}

		void set(bool enable)
		{
			// digitalWrite(pin_number, enable ? HIGH : LOW);
			pin->write(enable ? 1 : 0);
		}
		bool get() const
		{
			return 0 != pin->read();
		}
	private:
		int pin_number;
		mraa::Gpio* pin;
	};
}

#endif /* MYIOT_OUTPUT_H_ */
