/**
 * Copyright ©2021. Brent Weichel. All Rights Reserved.
 * Permission to use, copy, modify, and/or distribute this software, in whole
 * or part by any means, without express prior written agreement is prohibited.
 */
#pragma once

#include <cstdint>
#include <type_traits>

namespace Pique
{

/**
 * The base abstract class for hashing functions.
 * BlockSize is the length, in bytes, that the message is broken into before
 * the hashing procedure is performed. BlockSize is required to be greater than
 * zero. DigestSize is the length, in bytes, of the resulting hash digest.
 * A DigestSize of zero is reserved for hashing functions that let the user
 * choose the digest length.
 */
template < uint64_t BlockSize, uint64_t DigestSize >
class HashFunction
{
public:
	/**
	 * Constant used to denote that the digest is not a fixed size.
	 */
	static const uint64_t UNLIMITED_DIGEST_SIZE = 0;

	/**
	 * Length of the message block size, in bytes.
	 */
	static const uint64_t BLOCK_SIZE = BlockSize;

	/**
	 * Length of the hash function digest. If the length is zero, then
	 * the digest size is non-fixed.
	 */
	static const uint64_t DIGEST_SIZE = DigestSize;

	/**
	 * Default virtual destructor to ensure that the derived class's
	 * destructor will be called when using the abstract class.
	 */
	virtual ~HashFunction() = default;

	/**
	 * Compute the digest of the message and output to {@param messageDigest}.
	 * @param messageDigest Reference to an unsigned byte array of size DIGEST_SIZE.
	 */
	template < typename = typename std::enable_if< UNLIMITED_DIGEST_SIZE != DIGEST_SIZE >::type >
	virtual void digest( uint8_t ( &messageDigest )[ DIGEST_SIZE ] ) = 0;

	/**
	 * Compute the digest of the message and output to {@param messageDigest}.
	 * @param messageDigest Pointer to a byte array large enough to hold the requested length.
	 * @param digestSize Requested length of the digest, in bytes.
	 */
	template < typename = typename std::enable_if< UNLIMITED_DIGEST_SIZE == DIGEST_SIZE >::type >
	virtual void digest( uint8_t* messageDigest, uint64_t digestSize ) = 0;

	/**
	 * Compute the digest of the provided message without maintaining state information.
	 * @param messageDigest Reference to an unsigned byte array of size DIGEST_SIZE.
	 * @param message Pointer to an array of const bytes.
	 * @param messageLength Length of the message in bytes.
	 */
	template < typename = typename std::enable_if< UNLIMITED_DIGEST_SIZE != DIGEST_SIZE >::type >
	virtual static void digestMessage( uint8_t ( &messageDigest )[ DIGEST_SIZE ], const uint8_t* message, uint64_t messageLength ) = 0;

	/**
	 * Compute the digest of the provided message without maintaining state information.
	 * @param messageDigest Pointer to a byte array large enough to hold the requested length.
	 * @param digestSize Requested length of the digest, in bytes.
	 * @param message Pointer to an array of const bytes.
	 * @param messageLength Length of the message in bytes.
	 */
	template < typename = typename std::enable_if< UNLIMITED_DIGEST_SIZE == DIGEST_SIZE >::type >
	virtual static void digestMessage( uint8_t* messageDigest, uint64_t digestSize, const uint8_t* message, uint64_t messageLength ) = 0;

	/**
	 * Incorporate the provided message segment into the hash computation.
	 * @param message Pointer to an array of const bytes.
	 * @param messageLength Length of the message in bytes.
	 */
	virtual void update( const uint8_t* message, uint64_t messageLength ) = 0;

	/**
	 * Reset the internal state of the hash function to the initial state.
	 */
	virtual void reset() = 0;
};

} // namespace Pique
