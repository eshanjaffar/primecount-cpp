#include "primecount/algorithms/legendre.hpp"
#include <cstdint>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <utility>
#include <functional>
#include <cstddef>

namespace primecount
{
    // Getting the first 'a' primes, using a simple sieve
    static std::vector<uint64_t> generate_primes(uint64_t limit)
    {
        if (limit < 2) return {};
        std::vector<bool> is_prime(limit + 1, true);
        is_prime[0] = false;
        is_prime[1] = false;

        for (uint64_t p = 2; p <= limit/p; ++p)
        {
            if (is_prime[p])
            {
                for (uint64_t multiple = p*p; multiple <= limit; multiple += p)
                {
                    is_prime[multiple] = false;
                }
            }
        }

        std::vector<uint64_t>primes;
        for (uint64_t p = 2; p <= limit; ++p)
        {
            if (is_prime[p])primes.push_back(p);
        }

        return primes;
    }

    // Custom hash
    struct PhiKeyHash
    {
        std::size_t operator()(
            const std::pair<uint64_t, uint32_t>& key
        )const
        {
            const std::size_t h1 = std::hash<uint64_t>{}(key.first);
            const std::size_t h2 = std::hash<uint32_t>{}(key.second);

            return h1 ^ (h2 + + 0x9e3779b9U + (h1 << 6) + (h1 >> 2));
        }
    };

    //Memoization for phi
    using PhiCache = std::unordered_map<std::pair<uint64_t, uint32_t>, uint64_t, PhiKeyHash>;

    // recursive implimentation of phi(x, a)
    static uint64_t phi (uint64_t x, uint32_t a, const std::vector<uint64_t>& primes, PhiCache& cache)
    {
        if (a == 0) return x;

        // creating key
        const auto key = std::make_pair(x, a);

        // Checking if phi(x, a) has already been computed and stored in cache
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;

        // Storing the value of phi(x, a) in cache after computing it
        const uint64_t result = phi(x, a-1, primes, cache) - phi(x/primes[a - 1], a-1, primes, cache);
        cache[key] = result;

        return result;
    }


    // prime count using legendre sieve
    uint64_t prime_count_legendre(uint64_t n)
    {
        if (n < 2) return 0;

        // Computing the primes till sqrt(n)
        const uint64_t limit = static_cast<uint64_t>(std::sqrt(n));
        const std::vector<uint64_t> primes = generate_primes(limit);

        const uint32_t a = static_cast<uint32_t>(primes.size());

        // Creating an empty cache, and using the legendres identity for pi(x)
        PhiCache cache;
        return phi(n, a, primes, cache) + a - 1;
    }
}