
#include <gtest/gtest.h>
#include "../../include/myiot_timer_system.h"


class T_TimerSystem : public ::testing::Test
{
	virtual void SetUp()
	{

	}

	virtual void TearDown()
	{

	}

protected:
	MyIOT::TimerSystem sys;
};

TEST_F(T_TimerSystem, test_all_operators) {
	MyIOT::TimerSystem::TimeSpec tspec(1, 17);
	MyIOT::TimerSystem::TimeSpec tspec2(1, 17);
	MyIOT::TimerSystem::TimeSpec tspec3(2, 34);

	ASSERT_TRUE(tspec == tspec2);
	ASSERT_TRUE(tspec2 < tspec3);
	ASSERT_TRUE(tspec3 > tspec2);
	ASSERT_TRUE(tspec2 != tspec3);
}

TEST_F(T_TimerSystem, test_add_remove) {
	MyIOT::TimerSystem::TimeSpec tspec(1, 17);
	sys.add(new MyIOT::TimerSystem::CallbackTimer, tspec);
	ASSERT_TRUE(1 == sys.count());
	sys.reset();
	ASSERT_TRUE(0 == sys.count());

	MyIOT::TimerSystem::TimeSpec tspec2(1, 17);
	MyIOT::TimerSystem::TimeSpec tspec3(2, 34);
	sys.add(new MyIOT::TimerSystem::CallbackTimer, tspec2);
	sys.add(new MyIOT::TimerSystem::CallbackTimer, tspec3);
	ASSERT_TRUE(2 == sys.count());
}

TEST_F(T_TimerSystem, test_timer_expire)
{
	MyIOT::TimerSystem::CallbackTimer* tt = new MyIOT::TimerSystem::CallbackTimer([]{std::cout << "expired 1" << std::endl;});
	sys.add(tt, MyIOT::TimerSystem::TimeSpec(1,0));

	sys.expire(MyIOT::TimerSystem::TimeSpec(0,200000)); // this must be done to setup the next expiration.
	tt->reset(); // it will once expire!!!

	sys.expire(MyIOT::TimerSystem::TimeSpec(0,500000));
	ASSERT_FALSE(tt->is_expired());
	sys.expire(MyIOT::TimerSystem::TimeSpec(1,0));
	ASSERT_TRUE(tt->is_expired());
	tt->reset();
	sys.expire(MyIOT::TimerSystem::TimeSpec(1,0));
	ASSERT_FALSE(tt->is_expired());
	sys.expire(MyIOT::TimerSystem::TimeSpec(1,500000));
	ASSERT_FALSE(tt->is_expired());
	sys.expire(MyIOT::TimerSystem::TimeSpec(2,500000));
	ASSERT_TRUE(tt->is_expired());
}

TEST_F(T_TimerSystem, test_add_milliseconds_to_timespec)
{
	MyIOT::TimerSystem::TimeSpec x2ts(1,0);
	x2ts.add_milliseconds(500);
	ASSERT_TRUE(1 == x2ts.sec() && 500000000 == x2ts.nsec());
	x2ts.add_milliseconds(500);
	ASSERT_TRUE( 2 == x2ts.sec() && 0 == x2ts.nsec());
}

TEST_F(T_TimerSystem, test_run_loop)
{
	MyIOT::TimerSystem::CallbackTimer* it1 = new MyIOT::TimerSystem::CallbackTimer([]{std::cout << "1s expired" << std::endl;});
	sys.add(it1, MyIOT::TimerSystem::TimeSpec(0,100*1000*1000));
	sys.run_loop(100, 10);
	unsigned long count = it1->get_expire_counter();
	ASSERT_TRUE(10==count||11==count);
}

#if defined ORIGINAL_TEST
static void test()
{
	Serial.println("TimerSystem test -- started");
	TimerSystem sys;

	TimeSpec tspec(1, 17);
	TimeSpec tspec2(1, 17);
	TimeSpec tspec3(2, 34);

	T(1, tspec == tspec2);
	T(2, tspec2 < tspec3);
	T(3, tspec3 > tspec2);
	T(4, tspec2 != tspec3);

	sys.add(new CallbackTimer, tspec);
	T(5, 1 == sys.count());
	sys.reset();
	T(6, 0 == sys.count());

	sys.add(new CallbackTimer, tspec2);
	sys.add(new CallbackTimer, tspec3);
	T(7, 2 == sys.count());
	sys.reset();


	CallbackTimer* tt = new CallbackTimer([]{std::cout << "expired 1" << std::endl;});
	sys.add(tt, TimeSpec(1,0));

	sys.expire(TimeSpec(0,200000)); // this must be done to setup the next expiration.
	tt->reset(); // it will once expire!!!

	sys.expire(TimeSpec(0,500000));
	T(8, false == tt->is_expired());
	sys.expire(TimeSpec(1,0));
	T(9, true == tt->is_expired());
	tt->reset();
	sys.expire(TimeSpec(1,0));
	T(10, false == tt->is_expired());
	sys.expire(TimeSpec(1,500000));
	T(11, false == tt->is_expired());
	sys.expire(TimeSpec(2,500000));
	T(12, true == tt->is_expired());

	TimeSpec x2ts(1,0);
	x2ts.add_milliseconds(500);
	T(13, 1 == x2ts.sec() && 500000000 == x2ts.nsec());
	x2ts.add_milliseconds(500);
	T(14, 2 == x2ts.sec() && 0 == x2ts.nsec());


	sys.reset();
	CallbackTimer* it1 = new CallbackTimer([]{std::cout << "1s expired" << std::endl;});
	sys.add(it1, TimeSpec(0,100*1000*1000));
	sys.run_loop(100, 10);
    unsigned long count = it1->get_expire_counter();
	T(15, 10==count||11==count);
	Serial.print("TimerSystem test -- finished");
}

#endif

