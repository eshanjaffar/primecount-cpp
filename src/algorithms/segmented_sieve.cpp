#include "primecount/algorithms/segmented_sieve.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

namespace primecount
{
    // implementing a segmented odd prime sieve
    long long prime_count_segmented(long long n)
    {
        // Trivial case
        if (n < 2) return 0;

        const long long limit =
            static_cast<long long>(std::sqrt(n));

        // Generate primes up to sqrt(n)
        std::vector<bool> is_prime(limit + 1, true);

        // Initialising 0, 1 as not primes
        is_prime[0] = false;
        if (limit >= 1) is_prime[1] = false;

        for (long long p = 2; p <= limit / p; ++p)
        {
            if (is_prime[p])
            {
                for (long long multiple = p * p;
                     multiple <= limit;
                     multiple += p)
                {
                    is_prime[multiple] = false;
                }
            }
        }

        // Initialising the primes up to sqrt(n)
        std::vector<long long> base_primes;

        for (long long p = 2; p <= limit; ++p)
        {
            if (is_prime[p])
                base_primes.push_back(p);
        }

        // Count 2 separately; all remaining candidates are odd
        long long count = 1;

        // Each segment stores 32768 odd candidates
        constexpr long long segment_size = 32768;

        for (long long low = 3; low <= n;)
        {
            // Ensure low is odd
            if (low % 2 == 0) ++low;
            if (low > n) break;

            const long long high =
                std::min(n, low + 2 * (segment_size - 1));

            // Index i represents the odd number low + 2*i
            std::vector<bool> segment(
                (high - low) / 2 + 1, true
            );

            for (long long prime : base_primes)
            {
                // Skip 2: even numbers aren't stored
                if (prime == 2) continue;

                // No multiple needs marking if p*p > high
                if (prime > high / prime) break;

                long long start = low;
                const long long remainder = low % prime;

                if (remainder != 0)
                    start += prime - remainder;

                if (start < prime * prime)
                    start = prime * prime;

                // Make start odd
                if (start % 2 == 0)
                    start += prime;

                // Only odd multiples: advance by 2*p
                for (long long j = start; j <= high;)
                {
                    segment[(j - low) / 2] = false;

                    if (high - j < 2 * prime) break;
                    j += 2 * prime;
                }
            }

            // Count surviving odd candidates
            for (bool prime : segment)
            {
                if (prime) ++count;
            }

            if (high >= n - 1) break;

            // Start the next segment at the next odd number
            low = high + 1;
            if (low % 2 == 0) ++low;
        }

        return count;
    }
}
