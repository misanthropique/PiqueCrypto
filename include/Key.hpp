/**
 * Copyright ©2021. Brent Weichel. All Rights Reserved.
 * Permission to use, copy, modify, and/or distribute this software, in whole
 * or part by any means, without express prior written agreement is prohibited.
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <utility>

namespace Pique
{

/**
 * A class for holding a read-only buffer to a key
 * to be used for cryptograpihc functions.
 */
class Key final
{
private:
	typedef std::shared_key< const uint8_t > SharedKeyBuffer;

	mutable std::shared_mutex mKeyMutex;
	SharedKeyBuffer mKeyBuffer;
	size_t mKeyLength;

	SharedKeyBuffer
	__makeKeyBuffer( const uint8_t* data, size_t length )
	{
		if ( ( nullptr == data ) or ( 0 == length ) )
		{
			mKeyLength = 0;
			return SharedKeyBuffer( nullptr,
				[ = ]( const uint8_t* pointer )
				{
				} );
		}

		uint8_t* buffer = new uint8_t[ length ];
		mKeyLength = length;
		std::memcpy( buffer, data, length );

		return SharedKeyBuffer( buffer,
			[ = ]( const uint8_t* pointer )
			{
				std::memset( const_cast< uint8_t* >( pointer ), 0, length );
				delete[] pointer;
			} );
	}

public:
	/**
	 * Default construct a null key.
	 */
	Key() :
		mKeyBuffer( __makeKeyBuffer( nullptr, 0 ) )
	{
	}

	/**
	 * Copy constructor.
	 * @param other Constant reference to the Key object to copy.
	 */
	Key( const Key& other )
	{
		std::shared_lock keyReadLock( other.mKeyMutex );
		mKeyBuffer = other.mKeyBuffer;
		mKeyLength = other.mKeyLength;
	}

	/**
	 * Move constructor.
	 * @param other RValue to a Key object to assign to this new instance.
	 */
	Key( Key&& other )
	{
		std::unique_lock keyWriteLock( other.mKeyMutex );
		mKeyLength = std::exchange( other.mKeyLength, 0 );
		mKeyBuffer = std::move( other.mKeyBuffer );
	}

	/**
	 * Key value assignment constructor. If either {@param value} equals null or
	 * {@param length} equals zero, then this Key instance shall be null.
	 * @param value Pointer to an array of const uint8 data.
	 * @param length Length of {@param value} in bytes.
	 */
	Key( const uint8_t* value, size_t length ) :
		mKeyBuffer( __makeKeyBuffer( value, length ) )
	{
	}

	/**
	 * Key destructor.
	 */
	~Key()
	{
		mKeyLength = 0;
		mKeyBuffer.reset();
	}

	/**
	 * Clear this Key instance.
	 */
	void clear()
	{
		std::unique_lock keyWriteLock( mKeyMutex );
		mKeyBuffer = __makeKeyBuffer( nullptr, 0 );
	}

	/**
	 * Get a shared pointer to the const uint8_t buffer holding the key.
	 * @return A shared_ptr to the const uint8_t buffer is returned.
	 */
	std::shared_ptr< const uint8_t > key() const
	{
		std::shared_lock keyReadLock( mKeyMutex );
		return mKeyBuffer;
	}

	/**
	 * Get the length of the Key buffer in bytes.
	 * @return The length of the Key buffer in bytes.
	 */
	size_t length() const
	{
		std::shared_lock keyReadLock( mKeyMutex );
		return mKeyLength;
	}

	/**
	 * Copy assignment operator.
	 * @param other Constant reference to the Key object to copy.
	 * @return Reference to this Key instance is returned.
	 */
	Key& operator=( const Key& other )
	{
		if ( this != &other )
		{
			std::shared_lock keyReadLock( other.mKeyMutex );
			mKeyBuffer = other.mKeyBuffer;
			mKeyLength = other.mKeyLength;
		}

		return *this;
	}

	/**
	 * Move assignment operator.
	 * @param other RValue to a Key object to assign to this Key instance.
	 * @return Reference to this Key instance is returned.
	 */
	Key& operator=( Key&& other )
	{
		if ( this != &other )
		{
			std::lock( mKeyMutex, other.mKeyMutex );
			std::lock_guard thisKeyWriteLock( mKeyMutex, std::adopt_lock );
			std::lock_guard otherKeyWriteLock( other.mKeyMutex, std::adopt_lock );
			mKeyLength = std::exchange( other.mKeyLength, 0 );
			mKeyBuffer = std::move( other.mKeyBuffer );
		}

		return *this;
	}

	/**
	 * Check that this Key instance is equal to {@param other}.
	 * @param other Constant reference to a Key instance to compare against.
	 * @return True is returned if this Key instance contains the same content as {@param other}. False is otherwise returned.
	 */
	bool operator==( const Key& other ) const
	{
		std::scoped_lock keysReadLock( mKeyMutex, other.mKeyMutex );
		if ( mKeyLength == other.mKeyLength )
		{
			if ( ( nullptr == mKeyBuffer ) or ( nullptr == other.mKeyBuffer ) )
			{
				return ( mKeyBuffer == other.mKeyBuffer );
			}

			const uint8_t* buffer[ 2 ] = {
				mKeyBuffer.get(), other.mKeyBuffer.get() };
			for ( size_t index( mKeyLength ); index--; )
			{
				if ( buffer[ 0 ][ index ] != buffer[ 1 ][ index ] )
				{
					return false;
				}
			}

			return true;
		}

		return false;
	}

	/**
	 * Check that this Key instance is not equal to {@param other}.
	 * @param other Constant reference to a Key instance to compare against.
	 * @return False is returned if this Key instance contains the same content as {@param other}. True is otherwise returned.
	 */
	bool operator!=( const Key& other ) const
	{
		std::scoped_lock keysReadLock( mKeyMutex, other.mKeyMutex );
		if ( mKeyLength == other.mKeyLength )
		{
			if ( ( nullptr == mKeyBuffer ) or ( nullptr == other.mKeyBuffer ) )
			{
				return not ( mKeyBuffer == other.mKeyBuffer );
			}

			const uint8_t* buffer[ 2 ] = {
				mKeyBuffer.get(), other.mKeyBuffer.get() };
			for ( size_t index( mKeyLength ); index--; )
			{
				if ( buffer[ 0 ][ index ] != buffer[ 1 ][ index ] )
				{
					return true;
				}
			}

			return false;
		}

		return true;
	}

	/**
	 * Cast this Key instance to a boolean value.
	 * If the Key buffer is non-null, then True is returned, else False.
	 */
	operator bool() const
	{
		std::shared_lock keyReadLock( mKeyMutex );
		return not ( ( 0 == mKeyLength ) or ( nullptr == mKeyBuffer ) );
	}

	/**
	 * Cast this Key instance to a string.
	 * If the Key buffer is non-null, then the hexadecimal representation of the byte
	 * array is returned, else an empty string returned.
	 */
	operator std::string() const
	{
		static const char HEX_DIGIT[] = "0123456789abcdef";

		std::shared_lock keyReadLock( mKeyMutex );
		if ( ( 0 != mKeyLength ) and ( nullptr != mKeyBuffer ) )
		{
			const uint8_t* buffer = mKeyBuffer.get();
			std::string hexString( 2 * mKeyLength, ' ' );
			for ( size_t index( -1 ); ++index < mKeyLength; )
			{
				hexString[ 2 * index + 0 ] = HEX_DIGIT[ ( buffer[ index ] >> 4 ) & 0xF ];
				hexString[ 2 * index + 1 ] = HEX_DIGIT[ ( buffer[ index ] >> 0 ) & 0xF ];
			}

			return hexString;
		}

		return std::string( "" );
	}

	/**
	 * Set the value of this Key instance to the give key material.
	 * If {@param value} equals null or {@param length} equals zero, then
	 * this Key instance shall be null.
	 * @param value Pointer to an array of const uint8 data.
	 * @param length Length of {@param value} in bytes.
	 */
	void set( const uint8_t* value, size_t length )
	{
		std::unique_lock keyWriteLock( mKeyMutex );
		mKeyBuffer = __makeKeyBuffer( value, length );
	}
};

} // namespace Pique
