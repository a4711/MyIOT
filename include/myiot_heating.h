/*
 * myiot_heating.h
 *
 *  Created on: 29.04.2017
 *      Author: a4711
 */
#ifndef MYIOT_HEATING_H_
#define MYIOT_HEATING_H_
#include <list>
#include <stddef.h>

namespace MyIOT {
   namespace Heating
   {
      /// A interval consists of the length of the interval (milliseconds) and a end temperature (° celsius).
      /* Given a start temperature and a time within the interval, it will return the calculated current temperature.
       * The "time" in milliseconds will allow to store times within 24 days.
       * */
      class Interval
	  {
	  public:
    	  Interval(unsigned long aTime = 0, double aTemperature = 0.0): time(aTime), temperature(aTemperature)
	      {}

    	  Interval(const Interval& source): time(source.time), temperature(source.temperature)
    	  {}

    	  const Interval& operator=(const Interval& source)
    	  {
    		  time = source.time;
    		  temperature = source.temperature;
    		  return *this;
    	  }

    	  unsigned long getTime()const{return time;}
    	  double getTemperature()const{return temperature;}
    	  bool calculate(double startTemperature, unsigned long aTime, double& currentTemperature)
    	  {
    		  if (aTime>time) return false;

    		  double diff = temperature - startTemperature;
    		  double percentage = (double)aTime/double(time);
    		  double delta = diff * percentage;
    		  currentTemperature = startTemperature + delta;
    		  return true;
    	  }
	  private:
    	  unsigned long time;
    	  double temperature;
	  };


      /// The heating curve is defined by multiple heating intervals.
      class Curve
	  {
      public:
    	  Curve() : startTemperature(0.0){}
    	  void add(const Interval& interval)
    	  {
    		  intervals.push_back(interval);
    	  }

    	  void setStartTemperature(double aStartTemperature){startTemperature = aStartTemperature;}
    	  std::size_t count()const{return intervals.size();}

    	  bool calculate(unsigned long aTime, double& currentTemperature)
    	  {
    		  double start = startTemperature;
    		  for (Iterator i = intervals.begin(); i != intervals.end(); i++)
    		  {
    			  if ((*i).calculate(start, aTime, currentTemperature))
    			  {
    				  return true; // hit
    			  }
    			  else
    			  {
    				  // look at the next interval
    				  aTime -= (*i).getTime();
    				  start = (*i).getTemperature();
    			  }
    		  }
    		  return false;
    	  }

      private:
    	  Curve(const Curve&);
    	  const Curve& operator=(const Curve&);

    	  typedef std::list<Interval> IList;
    	  IList intervals;
    	  typedef IList::iterator Iterator;

    	  double startTemperature;
	  };

      /// Converter class to specify intervals in hours, minutes and seconds
      class Hours
      {
      public:
    	  Hours(unsigned long hoursValue, unsigned long minutesValue = 0, unsigned long secondsValue = 0)
		  : hours(hoursValue), minutes(minutesValue), seconds(secondsValue) {}

    	  operator unsigned long() const
		  {
    		  return  (3600*hours + 60*minutes + seconds) * 1000;
		  }
      private:
    	  Hours(const Hours&);
    	  const Hours& operator=(const Hours&);

    	  unsigned long hours;
    	  unsigned long minutes;
    	  unsigned long seconds;
      };
   }
}


#endif /* MYIOT_HEATING_H_ */
