///
/// @file   primegaps-c.cpp
/// @brief  C API wrapper for the PrimeGaps segmented-sieve engine.
///
#include "PrimeGapsGenerator.hpp"

#include <cerrno>
#include <exception>
#include <iostream>
#include <limits>

extern "C" int primesieve_primegaps(
    uint64_t runid,
    primesieve_primegaps_result* result)
{
  constexpr uint64_t RUN_SIZE = UINT64_C(100000000000);

  if (result == nullptr ||
      runid >= std::numeric_limits<uint64_t>::max() / RUN_SIZE)
  {
    errno = EINVAL;
    return -1;
  }

  const uint64_t start = runid * RUN_SIZE + 1;
  const uint64_t stop = (runid + 1) * RUN_SIZE;

  try
  {
    primesieve::PrimeGapsGenerator generator(start, stop);
    generator.sieve(*result);
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "primesieve_primegaps: " << e.what() << std::endl;
    errno = EDOM;
    return -1;
  }
}
