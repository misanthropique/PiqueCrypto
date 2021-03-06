#pragma once

#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>
#include <utility>

#include "Key.hpp"

TEST( TestKey, DefaultConstructorShallProduceANullKeyWithZeroLength )
{
	Key nullKey;

	ASSERT_EQ( nullptr, nullKey.mKeyBuffer );
	ASSERT_EQ( 0, nullKey.mKeyLength );
}

TEST( TestKey, AssignmentConstructorShallProduceANullKeyIfValueEqualsNullptrAndLengthEqualsZero )
{
	Key nullValueZeroLength( nullptr, 0 );

	ASSERT_EQ( nullptr, nullValueZeroLength.mKeyBuffer );
	ASSERT_EQ( 0, nullValueZeroLength.mKeyLength );
}

TEST( TestKey, AssignmentConstructorShallProduceANullKeyIfValueEqualsNullptrAndLengthDoesNotEqualZero )
{
	Key nullValueNonZeroLength( nullptr, 128 );

	ASSERT_EQ( nullptr, nullValueNonZeroLength.mKeyBuffer );
	ASSERT_EQ( 0, nullValueNonZeroLength.mKeyLength );
}

TEST( TestKey, AssignmentConstructorShallProduceANullKeyIfValueDoesNotEqualNullptrAndLengthEqualsZero )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

	Key nonNullValueZeroLength( nonNullValue, 0 );

	ASSERT_EQ( nullptr, nonNullValueZeroLength.mKeyBuffer );
	ASSERT_EQ( 0, nonNullValueZeroLength.mKeyLength );
}

TEST( TestKey, AssignmentConstructorShallProduceANonNullKeyIfValueDoesNotEqualNullptrAndLengthDoesNotEqualZero )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	ASSERT_EQ( nonNullKey.mKeyLength, nonNullValueLength );
	ASSERT_EQ( 0, std::memcmp( nonNullValue, nonNullKey.mKeyBuffer.get(), nonNullValueLength ) );
}

TEST( TestKey, CopyConstructorShallProduceANullKeyIfOtherIsANullKey )
{
	Key nullKey;
	Key copyNullKey( nullKey );

	ASSERT_EQ( nullptr, copyNullKey.mKeyBuffer );
	ASSERT_EQ( 0, copyNullKey.mKeyLength );
}

TEST( TestKey, CopyConstructorShallProduceANonNullKeyIfOtherIsANonNullKey )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );
	Key copyNonNullKey( nonNullKey );

	ASSERT_NE( nullptr, copyNonNullKey.mKeyBuffer );
	ASSERT_NE( 0, copyNonNullKey.mKeyLength );

	ASSERT_EQ( nonNullKey.mKeyLength, copyNonNullKey.mKeyLength );
	ASSERT_EQ( nonNullKey.mKeyBuffer, copyNonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, std::memcmp( nonNullKey.mKeyBuffer.get(), copyNonNullKey.mKeyBuffer.get(), copyNonNullKey.mKeyLength ) );
}

TEST( TestKey, MoveConstructorShallProduceANullKeyIfOtherIsANullKeyAndOtherShallBeNullAfterward )
{
	Key nullKey;

	ASSERT_EQ( nullptr, nullKey.mKeyBuffer );
	ASSERT_EQ( 0, nullKey.mKeyLength );

	Key moveNullKey( std::move( nullKey ) );

	ASSERT_EQ( nullptr, moveNullKey.mKeyBuffer );
	ASSERT_EQ( 0, moveNullKey.mKeyLength );

	ASSERT_EQ( nullptr, nullKey.mKeyBuffer );
	ASSERT_EQ( 0, nullKey.mKeyLength );
}

TEST( TestKey, MoveConstructorShallProduceANonNullKeyIfOtherIsANonNullKeyAndOtherShallBeNullAfterward )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nullKey.mKeyBuffer );
	ASSERT_NE( 0, nullKey.mKeyLength );

	Key moveNonNullKey( std::move( nonNullKey ) );

	ASSERT_NE( nullptr, moveNonNullKey.mKeyBuffer );
	ASSERT_NE( 0, moveNonNullKey.mKeyLength );
	ASSERT_EQ( nonNullValueLength, moveNonNullKey.mKeyLength );
	ASSERT_EQ( 0, std::memcmp( nonNullValue, moveNonNullKey.mKeyBuffer.get(), nonNullValueLength ) );

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
}
