/*
 * test_heating.cpp
 *
 *  Created on: 29.04.2017
 *      Author: a4711
 */


#include <gtest/gtest.h>

#include "../../include/myiot_heating.h"


namespace MyIOT
{
   namespace Heating
   {

      class Control
	  {
	  public:
    	  Control():curve(nullptr){}

    	  /// just tell if we should heat or not
    	  operator bool() const
    	  {
    		  if (nullptr == curve) return false;
    		  return false;
    	  }



    	  void init(const Curve* aCurve)
    	  {
    		  curve = aCurve;
    	  }



	  private:
    	  const Curve* curve;
    	  unsigned long lastMillis;
	  };

   }
}

using namespace MyIOT::Heating;

// --gtest_filter=T_Heating_Interval*
class T_Heating_Interval : public ::testing::Test
{

};

TEST_F(T_Heating_Interval, heating_interval_constructor)
{
	Interval i1(100, 33.3);

	ASSERT_EQ(100ul, i1.getTime());
	ASSERT_EQ(33.3, i1.getTemperature());
}


TEST_F(T_Heating_Interval, heating_interval_copy_constructor)
{
	Interval src(100, 33.3);
	Interval i1(src);

	ASSERT_EQ(100ul, i1.getTime());
	ASSERT_EQ(33.3, i1.getTemperature());
}

TEST_F(T_Heating_Interval, heating_interval_assignment)
{
	Interval src(100, 33.3);
	Interval i1;

	i1 = src;

	ASSERT_EQ(100ul, i1.getTime());
	ASSERT_EQ(33.3, i1.getTemperature());
}

TEST_F(T_Heating_Interval, heating_interval_given_a_start_temperature_and_a_time_calculates_current_temperature)
{
	Interval i(100ul, 200.0);
	double result = 0.0;
	ASSERT_TRUE(i.calculate(100.0, 50ul, result));
	ASSERT_EQ(150ul, result);
}

TEST_F(T_Heating_Interval, heating_interval_out_of_interval_returns_false)
{
	Interval i(100ul, 200.0);
	double result = 0.0;
    ASSERT_FALSE(i.calculate(100.0, 101ul, result));
}

TEST_F(T_Heating_Interval, heating_interval_t)
{}


class T_Heating_Curve : public ::testing::Test
{
public:
  virtual void SetUp()
  {
	  curve1.setStartTemperature(20.0);

		// 20° --(100ms)-> 40° --(100ms)-> 40° --(100ms) -> 20°
	  curve1.add(Interval(100ul, 40.0));
	  curve1.add(Interval(100ul, 40.0));
	  curve1.add(Interval(100ul, 20.0));
  }
protected:
	Curve curve1;
};

TEST_F(T_Heating_Curve, heating_curve_you_can_add_intervals_to_a_curve)
{
	Curve curve;
	curve.add(Interval(100ul,20));
	curve.add(Interval(100ul,30));
	curve.add(Interval(100ul,30));

	ASSERT_EQ(size_t(3), curve.count());

}

TEST_F(T_Heating_Curve, heating_curve_a_curve_can_have_a_start_temperature)
{
	Curve curve;
	curve.setStartTemperature(20.0);

}

TEST_F(T_Heating_Curve, heating_curve_you_can_calculate_a_current_temperature)
{
	Curve curve;
	curve.setStartTemperature(20.0);
	curve.add(Interval(100ul, 80.0));

	double currentTemperature = 0.0;
	ASSERT_TRUE(curve.calculate(25ul, currentTemperature));
	ASSERT_EQ(currentTemperature, 35.0);

	ASSERT_TRUE(curve.calculate(50ul, currentTemperature));
	ASSERT_EQ(currentTemperature, 50.0);

	ASSERT_TRUE(curve.calculate(75ul, currentTemperature));
	ASSERT_EQ(currentTemperature, 65.0);
}


TEST_F(T_Heating_Curve, heating_curve_out_of_interval_returns_false)
{
	double currentTemperature = 0.0;
	ASSERT_FALSE(curve1.calculate(301ul, currentTemperature));
}


TEST_F(T_Heating_Curve, heating_curve_check_multiple_intervals_20_40_40_20)
{
	double currentTemperature = 0.0;
	ASSERT_TRUE(curve1.calculate(0ul, currentTemperature));
	ASSERT_EQ(20, currentTemperature);

	ASSERT_TRUE(curve1.calculate(50ul, currentTemperature));
	ASSERT_EQ(30, currentTemperature);

	ASSERT_TRUE(curve1.calculate(100ul, currentTemperature));
	ASSERT_EQ(40, currentTemperature);

	ASSERT_TRUE(curve1.calculate(150ul, currentTemperature));
	ASSERT_EQ(40, currentTemperature);

	ASSERT_TRUE(curve1.calculate(200ul, currentTemperature));
	ASSERT_EQ(40, currentTemperature);

	ASSERT_TRUE(curve1.calculate(250ul, currentTemperature));
	ASSERT_EQ(30, currentTemperature);

	ASSERT_TRUE(curve1.calculate(300ul, currentTemperature));
	ASSERT_EQ(20, currentTemperature);
}



TEST_F(T_Heating_Curve, heating_curve_t)
{}


class T_Heating_Hours : public ::testing::Test
{
};


TEST_F(T_Heating_Hours, heating_hours_test_hours)
{
	unsigned long val = Hours(1ul);
	ASSERT_EQ(3600ul*1000ul, val);
	val = Hours(0ul, 61ul);
	ASSERT_EQ(61ul* 60ul*1000ul, val);
}

TEST_F(T_Heating_Hours, heating_hours_test_minutes)
{
	unsigned long val = Hours(0ul, 61ul);
	ASSERT_EQ((unsigned long)(61ul* 60ul*1000ul) , val);
}

TEST_F(T_Heating_Hours, heating_hours_test_seconds)
{
	unsigned long val = Hours(0ul, 0ul, 3ul);
	ASSERT_EQ(3000ul, val);
}

TEST_F(T_Heating_Hours, heating_hours_test_all)
{
	unsigned long val = Hours(1, 1, 1);
	ASSERT_EQ(3600000ul + 60000ul + 1000ul, val);
}

class T_Heating_Control : public ::testing::Test
{
public:
  virtual void SetUp() {
	  curve.setStartTemperature(20.0);

		// START : 20° --(4h)-> 800° --(15min)-> 800° --(2h) -> 1000° --(15min)-> 1000° : END
	  curve.add(Interval(Hours(4ul), 800.0));
	  curve.add(Interval(Hours(0ul,15ul), 800.0));
	  curve.add(Interval(Hours(2ul), 1000.0));
	  curve.add(Interval(Hours(0ul,15ul), 1000.0));

	  control.init(&curve);
  }

  virtual void TearDown() { }
protected:
  Curve curve;
  Control control;
};


#if defined HAS_FILE_SYSTEM
#include <fstream>
TEST_F(T_Heating_Control, heating_control_dump_curve_to_file)
{
	double temperature = 0.0;
	std::ofstream out("curve.txt");
	for (unsigned long millis = 0; ; millis += 1000)
	{
		if (!curve.calculate(millis, temperature)) break;
		out << millis/1000 << " : " << temperature << std::endl;
	}
	out.close();
}
#endif


class T_Heating : public ::testing::Test
{
  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};



