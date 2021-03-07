/**
 * Copyright ©2021. Brent Weichel. All Rights Reserved.
 * Permission to use, copy, modify, and/or distribute this software, in whole
 * or part by any means, without express prior written agreement is prohibited.
 */
#include <gtest/gtest.h>

#define private public

#include "Test_Key.hpp"

int main( int argc, char** argv )
{
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
