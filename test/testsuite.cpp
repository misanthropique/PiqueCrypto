#include <gtest/gtest.h>

#define private public

#include "Test_Key.hpp"

int main( int argc, char** argv )
{
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
