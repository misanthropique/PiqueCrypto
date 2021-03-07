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
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nullKey;
	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	nonNullKey = std::move( nullKey );

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
	ASSERT_EQ( nullptr, nullKey.mKeyBuffer );
	ASSERT_EQ( 0, nullKey.mKeyLength );
}

TEST( TestKey, MoveAssignmentShallSetNullKeyToNonNullIfOtherIsANonNullKeyAndOtherShallBeANullKeyAfterward )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nullKey;
	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_EQ( nullptr, nullKey.mKeyBuffer );
	ASSERT_EQ( 0, nullKey.mKeyLength );
	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	nullKey = std::move( nonNullKey );

	ASSERT_NE( nullptr, nullKey.mKeyBuffer );
	ASSERT_NE( 0, nullKey.mKeyLength );
	ASSERT_EQ( nonNullValueLength, nullKey.mKeyLength );
	ASSERT_EQ( 0, std::memcmp( nonNullValue, nullKey.mKeyBuffer.get(), nonNullValueLength ) );

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
}

TEST( TestKey, MoveAssignmentShallSetNonNullKeyToNonNullIfOtherIsANonNullKeyAndOtherShallBeANullKeyAfterward )
{
	static const uint8_t[] nonNullValueA = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueALength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	static const uint8_t[] nonNullValueB = { 0x03, 0x02, 0x01, 0x00 };
	static const size_t nonNullValueBLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKeyA( nonNullValueA, nonNullValueALength );
	Key nonNullKeyB( nonNullValueB, nonNullValueBLength );

	ASSERT_NE( nullptr, nonNullKeyA.mKeyBuffer );
	ASSERT_NE( 0, nonNullKeyA.mKeyLength );
	ASSERT_NE( nullptr, nonNullKeyB.mKeyBuffer );
	ASSERT_NE( 0, nonNullKeyB.mKeyLength );

	nonNullKeyA = std::move( nonNullKeyB );

	ASSERT_NE( nullptr, nonNullKeyA.mKeyBuffer );
	ASSERT_NE( 0, nonNullKeyA.mKeyLength );
	ASSERT_EQ( nonNullValueBLength, nonNullKeyA.mKeyLength );
	ASSERT_EQ( 0, std::memcmp( nonNullValueB, nonNullKeyA.mKeyBuffer.get(), nonNullValueBLength ) );

	ASSERT_EQ( nullptr, nonNullKeyB.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKeyB.mKeyLength );
}

TEST( TestKey, EqualityOperatorShallReturnFalseIfKeyLengthsAreNotEqual )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKeyA( nonNullValue, nonNullValueLength );
	Key nonNullKeyB( nonNullValue, nonNullValueLength );

	nonNullKeyB.mKeyLength = nonNullKeyA.mKeyLength + 1;

	ASSERT_FALSE( nonNullKeyA == nonNullKeyB );
}

TEST( TestKey, EqualityOperatorShallReturnTrueIfBothKeysAreNull )
{
	Key nullKeyA;
	Key nullKeyB;

	ASSERT_TRUE( nullKeyA, nullKeyB );
}

TEST( TestKey, EqualityOperatorShallReturnFalseIfBothKeysAreOfEqualLengthAndDifferingContent )
{
	static const uint8_t[] nonNullValueA = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueALength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	static const uint8_t[] nonNullValueB = { 0x00, 0x01, 0x02, 0x03, 0x07, 0x06, 0x05, 0x04 };
	static const size_t nonNullValueBLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	ASSERT_EQ( nonNullValueALength, nonNullValueBLength );
	ASSERT_NE( 0, std::memcmp( nonNullValueA, nonNullValueB, nonNullValueALength ) );

	Key nonNullKeyA( nonNullValueA, nonNullValueALength );
	Key nonNullKeyB( nonNullValueB, nonNullValueBLength );

	ASSERT_EQ( nonNullKeyA.mKeyLength, nonNullKeyB.mKeyLength );
	ASSERT_FALSE( nonNullKeyA == nonNullKeyB );
}

TEST( TestKey, EqualityOperatorShallReturnTrueIfBothKeysAreOfEqualLengthAndEqualContent )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKeyA( nonNullValue, nonNullValueLength );
	Key nonNullKeyB( nonNullValue, nonNullValueLength );

	ASSERT_EQ( nonNullKeyA.mKeyLength, nonNullKeyB.mKeyLength );
	ASSERT_EQ( 0, std::memcmp( nonNullKeyA.mKeyBuffer.get(), nonNullKeyB.mKeyBuffer.get(), nonNullValueLength ) );
	ASSERT_TRUE( nonNullKeyA == nonNullKeyB );
}

TEST( TestKey, InequalityOperatorShallReturnTrueIfKeyLengthsAreNotEqual )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nullKey;
	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullKey.mKeyLength, nonNullKey.mKeyLength );
	ASSERT_TRUE( nullKey != nonNullKey );
}

TEST( TestKey, InequalityOperatorShallReturnFalseIfBothKeysAreNull )
{
	Key nullKeyA;
	Key nullKeyB;

	ASSERT_FALSE( nullKeyA != nullKeyB );
}

TEST( TestKey, InequalityOperatorShallReturnTrueIfBothKeysAreOfEqualLengthAndDifferingContent )
{
	static const uint8_t[] nonNullValueA = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueALength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	static const uint8_t[] nonNullValueB = { 0x00, 0x01, 0x02, 0x03, 0x07, 0x06, 0x05, 0x04 };
	static const size_t nonNullValueBLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	ASSERT_EQ( nonNullValueALength, nonNullValueBLength );
	ASSERT_NE( 0, std::memcmp( nonNullValueA, nonNullValueB, nonNullValueALength ) );

	Key nonNullKeyA( nonNullValueA, nonNullValueALength );
	Key nonNullKeyB( nonNullValueB, nonNullValueBLength );

	ASSERT_EQ( nonNullKeyA.mKeyLength, nonNullKeyB.mKeyLength );
	ASSERT_NE( 0, std::memcmp( nonNullKeyA.mKeyBuffer.get(), nonNullKeyB.mKeyBuffer.get(), nonNullKeyA.mKeyLength ) );
	ASSERT_TRUE( nonNullKeyA != nonNullKeyB );
}

TEST( TestKey, InequalityOperatorShallReturnFalseIfBothKeysAreOfEqualLengthAndEqualContent )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKeyA( nonNullValue, nonNullValueLength );
	Key nonNullKeyB( nonNullValue, nonNullValueLength );

	ASSERT_EQ( nonNullKeyA.mKeyLength, nonNullKeyB.mKeyLength );
	ASSERT_EQ( 0, std::memcmp( nonNullKeyA.mKeyBuffer.get(), nonNullKeyB.mKeyBuffer.get(), nonNullKeyA.mKeyLength ) );
	ASSERT_FALSE( nonNullKeyA != nonNullKeyB );
}

TEST( TestKey, BoolOperatorShallReturnFalseIfKeyIsNull )
{
	Key nullKey;

	ASSERT_FALSE( bool( nullKey ) );
}

TEST( TestKey, BoolOperatorShallReturnTrueIfKeyIsNull )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_TRUE( bool( nonNullKey ) );
}

TEST( TestKey, StringOperatorShallReturnAnEmptyStringForANullKey )
{
	Key nullKey;

	std::string keyString( nullKey );

	ASSERT_TRUE( keyString == "" );
}

TEST( TestKey, StringOperatorShallReturnTheHexadecimalRepresentationOfTheKeyContentIfKeyIsNonNull )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );
	static const std::string nonNullValueString( "0001020304050607" );

	Key nonNullKey( nonNullValue, nonNullValueLength );
	std::string keyString( nonNullKey );

	ASSERT_EQ( nonNullValueLength * 2, keyString.size() );
	ASSERT_TRUE( keyString == nonNullValueString );
}

TEST( TestKey, SetShallSetTheKeyToNullIfValueIsNullAndLengthIsZero )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	nonNullKey.set( nullptr, 0 );

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
}

TEST( TestKey, SetShallSetTheKeyToNullIfValueIsNullAndLengthIsNonZero )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	nonNullKey.set( nullptr, 128 );

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
}

TEST( TestKey, SetShallSetTheKeyToNullIfLengthIsZeroAndValueIsNonNull )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nonNullKey( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_NE( 0, nonNullKey.mKeyLength );

	nonNullKey.set( nonNullValue, 0 );

	ASSERT_EQ( nullptr, nonNullKey.mKeyBuffer );
	ASSERT_EQ( 0, nonNullKey.mKeyLength );
}

TEST( TestKey, SetShallSetTheKeyToNonNullIfValueIsNonNullAndLengthIsNonZero )
{
	static const uint8_t[] nonNullValue = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
	static const size_t nonNullValueLength = sizeof( nonNullValue ) / sizeof( *nonNullValue );

	Key nullKey;

	ASSERT_EQ( nullptr, nullKey.mKeyBuffer );
	ASSERT_EQ( 0, nullKey.mKeyLength );

	nullKey.set( nonNullValue, nonNullValueLength );

	ASSERT_NE( nullptr, nullKey.mKeyBuffer );
	ASSERT_NE( 0, nullKey.mKeyLength );
	ASSERT_EQ( nullKey.mKeyLength, nonNullValueLength );
	ASSERT_EQ( 0, std::memcmp( nullKey.mKeyBuffer.get(), nonNullValue, nonNullValueLength ) );
}
