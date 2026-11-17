///
/// @file   PrimeGapsGenerator.cpp
/// @brief  Calculates prime gaps while processing segmented sieve data.
///
#include "PrimeGapsGenerator.hpp"
#include "SievingPrimes.hpp"

#include <primesieve/pmath.hpp>
#include <primesieve/util.hpp>

#include <algorithm>

namespace primesieve {

namespace {
/// Maximum sieve array size passed to Erat, in KiB.
/// Erat may select a smaller effective segment based on CPU cache sizes.
constexpr uint64_t PRIMEGAPS_SIEVE_SIZE_KIB = 64;
}

PrimeGapsGenerator::PrimeGapsGenerator(uint64_t start, uint64_t stop)
{
  Erat::init(start, stop, PRIMEGAPS_SIEVE_SIZE_KIB, memoryPool_);
}

void PrimeGapsGenerator::recordPrime(uint64_t prime)
{
  if (primeCount_ == 0)
  {
    firstPrime_ = prime;
  }
  else
  {
    uint64_t gap = prime - previousPrime_;
    // Keep the first occurrence when multiple gaps have the same maximum.
    if (gap > maxGap_)
    {
      maxGap_ = gap;
      maxGapStart_ = previousPrime_;
    }
  }

  previousPrime_ = prime;
  primeCount_++;
}

void PrimeGapsGenerator::processSegment(uint64_t low)
{
  for (std::size_t i = 0; i < sieve_.size(); i++)
  {
    uint64_t bits = to_littleendian(sieve_[i]);

    while (bits != 0)
    {
      recordPrime(nextPrime(bits, low));
      bits &= bits - 1;
    }

    low += 8 * 30;
  }
}

void PrimeGapsGenerator::sieve(primesieve_primegaps_result& result)
{
  // SievingPrimes generates the base primes needed to sieve each segment.
  SievingPrimes sievingPrimes(this, PRIMEGAPS_SIEVE_SIZE_KIB, memoryPool_);
  uint64_t prime = sievingPrimes.next();

  while (hasNextSegment())
  {
    const uint64_t low = segmentLow_;
    const uint64_t sqrtHigh = isqrt(segmentHigh_);

    for (; prime <= sqrtHigh; prime = sievingPrimes.next())
      addSievingPrime(prime);

    sieveSegment();
    processSegment(low);
  }

  result.max_gap = maxGap_;
  result.max_gap_start = maxGapStart_;
  result.prime_count = primeCount_;
  result.first_gap = primeCount_ ? firstPrime_ - start_ : 0;
  result.last_gap = primeCount_ ? stop_ - previousPrime_ : 0;
}

} // namespace primesieve
