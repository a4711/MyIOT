/*
 * test_control_time.cpp
 *
 *  Created on: 23.04.2017
 *      Author: a4711
 */

#include <gtest/gtest.h>

#include "../../include/myiot_control_time.h"

class T_ControlTime : public ::testing::Test
{
};


TEST_F(T_ControlTime, complete_test)
{
	MyIOT::ControlTime tm(MyIOT::ControlTime::HourMinuteSecond(8, 0),
			MyIOT::ControlTime::HourMinuteSecond(9, 0));

	ASSERT_TRUE(tm.check(MyIOT::ControlTime::HourMinuteSecond(8, 30)));
	ASSERT_FALSE(tm.check(MyIOT::ControlTime::HourMinuteSecond(9, 1)));
	ASSERT_FALSE(tm.check(MyIOT::ControlTime::HourMinuteSecond(7, 59)));
	ASSERT_TRUE(tm.check(MyIOT::ControlTime::HourMinuteSecond(8, 0)));
	ASSERT_TRUE(tm.check(MyIOT::ControlTime::HourMinuteSecond(9, 0)));

	MyIOT::ControlTime tm2(MyIOT::ControlTime::HourMinuteSecond(9, 0),
			MyIOT::ControlTime::HourMinuteSecond(8, 0));

	ASSERT_FALSE(tm2.check(MyIOT::ControlTime::HourMinuteSecond(8, 30))
					&& !tm2.check(
							MyIOT::ControlTime::HourMinuteSecond(8, 1))
					&& !tm2.check(
							MyIOT::ControlTime::HourMinuteSecond(8, 59)));
	ASSERT_TRUE(tm2.check(MyIOT::ControlTime::HourMinuteSecond(9, 1)));
	ASSERT_TRUE(tm2.check(MyIOT::ControlTime::HourMinuteSecond(7, 59)));
	ASSERT_TRUE(tm2.check(MyIOT::ControlTime::HourMinuteSecond(8, 0)));
	ASSERT_TRUE(tm2.check(MyIOT::ControlTime::HourMinuteSecond(9, 0)));
}


#if defined ORIGINAL_TEST
static void test()
{
	MyIOT::ControlTime tm(MyIOT::ControlTime::HourMinuteSecond(8, 0),
			MyIOT::ControlTime::HourMinuteSecond(9, 0));

	T(1, tm.check(MyIOT::ControlTime::HourMinuteSecond(8, 30)));
	T(2, !tm.check(MyIOT::ControlTime::HourMinuteSecond(9, 1)));
	T(3, !tm.check(MyIOT::ControlTime::HourMinuteSecond(7, 59)));
	T(4, tm.check(MyIOT::ControlTime::HourMinuteSecond(8, 0)));
	T(5, tm.check(MyIOT::ControlTime::HourMinuteSecond(9, 0)));

	MyIOT::ControlTime tm2(MyIOT::ControlTime::HourMinuteSecond(9, 0),
			MyIOT::ControlTime::HourMinuteSecond(8, 0));

	T(6,
			!tm2.check(MyIOT::ControlTime::HourMinuteSecond(8, 30))
					&& !tm2.check(
							MyIOT::ControlTime::HourMinuteSecond(8, 1))
					&& !tm2.check(
							MyIOT::ControlTime::HourMinuteSecond(8, 59)));
	T(7, tm2.check(MyIOT::ControlTime::HourMinuteSecond(9, 1)));
	T(8, tm2.check(MyIOT::ControlTime::HourMinuteSecond(7, 59)));
	T(9, tm2.check(MyIOT::ControlTime::HourMinuteSecond(8, 0)));
	T(10, tm2.check(MyIOT::ControlTime::HourMinuteSecond(9, 0)));
}
#endif

