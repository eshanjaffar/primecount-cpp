#include "primecount/algorithms/trial_division.hpp"

namespace primecount
{

    // Basic function to check primality
    bool is_prime(long long n)
    {
        if (n <= 1) return false;
        
        for (long long d = 2; d*d <= n; ++d)
        {
            if (n % d == 0) return false;

        }
        return true;
    }

    // Basic pi(x) implimentation using is_prime
    long long prime_count(long long n)
    {
        long long count = 0;
        for (long long i = 2; i <= n; i++)
        {
            if(is_prime(i))count++;
        }

        return count;
    }
}
