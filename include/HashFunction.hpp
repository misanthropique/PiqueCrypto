/**
 * Copyright ©2021. Brent Weichel. All Rights Reserved.
 * Permission to use, copy, modify, and/or distribute this software, in whole
 * or part by any means, without express prior written agreement is prohibited.
 */
#pragma once

#include <cstdint>

namespace Pique
{

/**
 * The base abstract class for hashing functions.
 * BlockSize shall be the number of bits in a block for
 * the hash functions and shall be a power of 2 greater than zero.
 * DigestSize shall be the number of bits in the digest and
 * shall be a power of 2, or zero. A DigestSize of zero shall
 * be reserved for hashing functions that have an unbounded digest size.
 */
template < uint64_t BlockSize, uint64_t DigestSize,
	typename = typename std::enable_if<
		( 0 < BlockSize ) and ( 0 == ( BlockSize bitand ( BlockSize - 1 ) ) )
		and ( 0 == ( DigestSize bitand ( DigestSize - 1 ) ) )
	>::type >
class HashFunction
{
public:
	static const uint64_t UNBOUNDED_DIGEST = 0;

	static const uint64_t BLOCK_SIZE = BlockSize;

	static const uint64_t DIGEST_SIZE = DigestSize;

	virtual ~HashFunction() = default;

	/**
	 * Compute the digest of the message and output to {@param messageDigest}.
	 * @param messageDigest Reference to an unsigned byte array of size DIGEST_SIZE.
	 */
	template < typename = typename std::enable_if< UNBOUNDED_DIGEST != DIGEST_SIZE >::type >
	virtual void digest( uint8_t ( &messageDigest )[ DIGEST_SIZE ] ) = 0;

	/**
	 * Compute the digest of the message to a bit length of
	 * {@param digestSize} and output to {@param messageDigest}.
	 * @param messageDigest Pointer to a byte array long enough to store the requested digest.
	 * @param digestSize The bit-length of the digest to be computed.
	 */
	template < typename = typename std::enable_if< UNBOUNDED_DIGEST == DIGEST_SIZE >::type >
	virtual void digest( uint8_t* messageDigest, uint64_t digestSize ) = 0;

	template < typename = typename std::enable_if< UNBOUNDED_DIGEST != DIGEST_SIZE >::type >
	virtual static void digestMessage( uint8_t ( &messageDigest )[ DIGEST_SIZE ], const uint8_t* message, ) = 0;

	template < typename = typename std::enable_if< UNBOUNDED_DIGEST == DIGEST_SIZE >::type >
	virtual static void digestMessage() = 0;

	virtual void update( const uint8_t* message, uint64_t 
};

} // namespace Pique
