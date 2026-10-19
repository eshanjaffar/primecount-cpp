#include "primecount/algorithms/sieve.hpp"
#include <vector>


namespace primecount
{
    // simple odd only sieve implimentation
    long long prime_count_sieve(long long n)
    {
        // Trivial case
        if (n < 2) return 0;

        //only need to consider the n/2 + 1 odd numbers
        std::vector<bool> is_prime(n/2 + 1, true);

        // setting 1 as false
        is_prime[0] = false;


        for (long long p = 3; p <= n/p ; p+= 2)
        {
            if(is_prime[p/2])
            {
                for (long long multiple = p*p; multiple <= n; multiple += 2*p)
                {
                    is_prime[multiple/2] = false;
                }
            }
        }

        // since 2 is prime, and we arent considering 2 in array is_prime
        int count = 1;

        // Count the remaining odd primes.
        for (long long i = 1; i < static_cast<long long>(is_prime.size()); ++i)
        {
            if (is_prime[i])
            {
                long long value = 2 * i + 1;
                if (value <= n)
                {
                    ++count;
                }
            }
        }

        return count;

    }
}