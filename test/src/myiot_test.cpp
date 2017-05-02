//============================================================================


// Name        : myiot_test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <gtest/gtest.h>



TEST(test_case_name, test_name) {
 ASSERT_TRUE( true);
}


int main(int argc, char** argv ) {


  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
