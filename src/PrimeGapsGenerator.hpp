///
/// @file  PrimeGapsGenerator.hpp
/// @brief Computes PrimeGaps statistics directly from primesieve's segmented sieve.
///
/// This class is intentionally single-threaded. Parallelism is provided by
/// running independent PrimeGaps processes for different run IDs.
///
#ifndef PRIMEGAPSGENERATOR_HPP
#define PRIMEGAPSGENERATOR_HPP

#include "Erat.hpp"
#include "MemoryPool.hpp"

#include <primesieve.h>
#include <stdint.h>

namespace primesieve {

class PrimeGapsGenerator : public Erat
{
public:
  PrimeGapsGenerator(uint64_t start, uint64_t stop);
  void sieve(primesieve_primegaps_result& result);

private:
  MemoryPool memoryPool_;
  uint64_t firstPrime_ = 0;
  uint64_t previousPrime_ = 0;
  uint64_t primeCount_ = 0;
  uint64_t maxGap_ = 0;
  uint64_t maxGapStart_ = 0;

  void recordPrime(uint64_t prime);
  void processSegment(uint64_t low);
};

} // namespace primesieve

#endif
