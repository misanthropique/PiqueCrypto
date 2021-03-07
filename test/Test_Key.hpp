/**
 * Copyright ©2021. Brent Weichel. All Rights Reserved.
 * Permission to use, copy, modify, and/or distribute this software, in whole
 * or part by any means, without express prior written agreement is prohibited.
 */
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

	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	Key moveNonNullKey( std::move( nonNullKey ) );

	ASSERT_NE( nullptr, moveNonNullKey.mKeyBuffer );
	ASSERT_NE( 0, moveNonNullKey.mKeyLength );
	ASSERT_EQ( nonNullValueLength, moveNonNullKey.mKeyLength );
	ASSERT_EQ( 0, std::memcmp( nonNullValue, moveNonNullKey.mKeyBuffer.get(), nonNullValueLength ) );

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
}

TEST( TestKey, ClearShallSetTheKeyAsNull )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	nonNullKey.clear();

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
}

TEST( TestKey, KeyShallReturnANullSharedPtrIfKeyIsNull )
{
	Key nullKey;

	ASSERT_EQ( nullptr, nullKey.key() );
}

TEST( TestKey, KeyShallReturnANonNullSharedPtrIfKeyIsNonNull )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nonNullKey.key() );
}

TEST( TestKey, LengthShallReturnZeroForNullKey )
{
	Key nullKey;

	ASSERT_EQ( 0, nullKey.length() );
}

TEST( TestKey, LengthShallReturnNonZeroForNonNullKey )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( 0, nonNullKey.length() );
	ASSERT_EQ( nonNullValueLength, nonNullKey.length() );
}

TEST( TestKey, CopyAssignmentOperatorShallSetKeyToNullIfOtherIsANullKey )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );
	Key nullKey;

	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	nonNullKey = nullKey;

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
}

TEST( TestKey, CopyAssignmentOperatorShallSetNullKeyToNonNullIfOtherIsANonNullKey )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nullKey;
	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_EQ( nullptr, nullKey.mKeyBuffer );
	ASSERT_EQ( 0, nullKey.mKeyLength );

	nullKey = nonNullKey;

	ASSERT_NE( nullptr, nullKey.mKeyBuffer );
	ASSERT_NE( 0, nullKey.mKeyLength );
	ASSERT_EQ( nonNullKey.mKeyLength, nullKey.mKeyLength );
	ASSERT_EQ( 0, std::memcmp( nullKey.mKeyBuffer.get(), nonNullKey.mKeyBuffer.get(), nullKey.mKeyLength ) );
	ASSERT_EQ( nullKey.mKeyBuffer, nonNullKeyBuffer.mKeyBuffer );
}

TEST( TestKey, CopyAssignmentOperatorShallSetNonNullKeyToNonNullIfOtherIsANonNullKey )
{
	static const uint8_t[] nonNullValueA = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueALength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	static const uint8_t[] nonNullValueB = { 0x03, 0x02, 0x01, 0x00 };
	static const size_t nonNullValueBLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKeyA( nonNullValueA, nonNullValueALength );
	Key nonNullKeyB( nonNullValueB, nonNullValueBLength );

	ASSERT_NE( nonNullKeyA.mKeyBuffer, nonNullKeyB.mKeyBuffer );

	nonNullKeyA = nonNullKeyB;

	ASSERT_EQ( nonNullKeyA.mKeyLength, nonNullKeyB.mKeyLength );
	ASSERT_EQ( 0, std::memcmp( nonNullKeyA.mKeyBuffer.get(), nonNullKeyB.mKeyBuffer.get(), nonNullKeyA.mKeyLength ) );
	ASSERT_EQ( nonNullKeyA.mKeyBuffer, nonNullKeyB.mKeyBuffer );
}

TEST( TestKey, MoveAssignmentShallSetNonNullKeyToNullIfOtherIsANullKeyAndOtherShallBeANullKeyAfterward )
{
}

TEST( TestKey, MoveAssignmentShallSetNullKeyToNonNullIfOtherIsANonNullKeyAndOtherShallBeANullKeyAfterward )
{
}

TEST( TestKey, MoveAssignmentShallSetNonNullKeyToNonNullIfOtherIsANonNullKeyAndOtherShallBeANullKeyAfterward )
{
}

TEST( TestKey, EqualityOperatorShallReturnFalseIfKeyLengthsAreNotEqual )
{
}

TEST( TestKey, EqualityOperatorShallReturnTrueIfBothKeysAreNull )
{
}

TEST( TestKey, EqualityOperatorShallReturnFalseIfBothKeysAreOfEqualLengthAndDifferingContent )
{
}

TEST( TestKey, EqualityOperatorShallReturnTrueIfBothKeysAreOfEqualLengthAndEqualContent )
{
}

TEST( TestKey, InequalityOperatorShallReturnTrueIfKeyLengthsAreNotEqual )
{
}

TEST( TestKey, InequalityOperatorShallReturnFalseIfBothKeysAreNull )
{
}

TEST( TestKey, InequalityOperatorShallReturnTrueIfBothKeysAreOfEqualLengthAndDifferingContent )
{
}

TEST( TestKey, InequalityOperatorShallReturnFalseIfBothKeysAreOfEqualLengthAndEqualContent )
{
}

TEST( TestKey, BoolOperatorShallReturnFalseIfKeyIsNull )
{
}

TEST( TestKey, BoolOperatorShallReturnTrueIfKeyIsNull )
{
}

TEST( TestKey, StringOperatorShallReturnAnEmptyStringForANullKey )
{
}

TEST( TestKey, StringOperatorShallReturnTheHexadecimalRepresentationOfTheKeyContentIfKeyIsNonNull )
{
}

TEST( TestKey, SetShallSetTheKeyToNullIfValueIsNullAndLengthIsZero )
{
}

TEST( TestKey, SetShallSetTheKeyToNullIfValueIsNullAndLengthIsNonZero )
{
}

TEST( TestKey, SetShallSetTheKeyToNullIfLengthIsZeroAndValueIsNonNull )
{
}

TEST( TestKey, SetShallSetTheKeyToNonNullIfValueIsNonNullAndLengthIsNonZero )
{
}
