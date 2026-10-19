
#include "primecount/algorithms/lehmer.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace primecount
{
    namespace
    {
        constexpr uint32_t MAX_SIEVE_LIMIT = 10'000'000;
        constexpr uint32_t CACHE_A_LIMIT = 128;

        struct PrimeData
        {
            std::vector<uint32_t> primes;
            std::vector<uint32_t> pi_table;
        };

        struct PhiKey
        {
            uint64_t x;
            uint32_t a;

            bool operator==(const PhiKey& other) const
            {
                return x == other.x && a == other.a;
            }
        };

        struct PhiKeyHash
        {
            size_t operator()(const PhiKey& key) const
            {
                size_t h1 = std::hash<uint64_t>{}(key.x);
                size_t h2 = std::hash<uint32_t>{}(key.a);

                return h1 ^ (h2 + 0x9e3779b9U
                             + (h1 << 6) + (h1 >> 2));
            }
        };

        using PhiCache =
            std::unordered_map<PhiKey, uint64_t, PhiKeyHash>;

        using PiCache = std::unordered_map<uint64_t, uint64_t>;

        PrimeData build_prime_data(uint32_t limit)
        {
            PrimeData data;
            data.pi_table.resize(
                static_cast<size_t>(limit) + 1, 0
            );

            if (limit < 2)
                return data;

            data.primes.push_back(2);

            // Index i represents the odd number 2*i + 3.
            const uint32_t size = (limit - 1) / 2;
            std::vector<bool> is_prime(size, true);

            for (uint32_t i = 0; i < size; ++i)
            {
                const uint64_t p = 2ULL * i + 3;

                if (p > limit / p)
                    break;

                if (is_prime[i])
                {
                    uint64_t j = (p * p - 3) / 2;

                    for (; j < size; j += p)
                        is_prime[static_cast<size_t>(j)] = false;
                }
            }

            for (uint32_t i = 0; i < size; ++i)
            {
                if (is_prime[i])
                    data.primes.push_back(2 * i + 3);
            }

            size_t prime_index = 0;

            for (uint32_t x = 0; x <= limit; ++x)
            {
                if (prime_index < data.primes.size()
                    && data.primes[prime_index] == x)
                {
                    ++prime_index;
                }

                data.pi_table[x] =
                    static_cast<uint32_t>(prime_index);
            }

            return data;
        }

        uint64_t isqrt(uint64_t n)
        {
            uint64_t r =
                static_cast<uint64_t>(std::sqrt(
                    static_cast<long double>(n)));

            while (r != 0 && r > n / r)
                --r;

            while (r < UINT64_MAX
                   && r + 1 <= n / (r + 1))
            {
                ++r;
            }

            return r;
        }

        uint64_t icbrt(uint64_t n)
        {
            uint64_t r =
                static_cast<uint64_t>(std::cbrt(
                    static_cast<long double>(n)));

            while (r > 0 && r > n / r / r)
                --r;

            while (r < UINT64_MAX)
            {
                const uint64_t next = r + 1;

                if (next > n / next / next)
                    break;

                ++r;
            }

            return r;
        }

        uint64_t iroot4(uint64_t n)
        {
            return isqrt(isqrt(n));
        }

        uint64_t pi_lookup(
            uint64_t x,
            const std::vector<uint32_t>& pi_table)
        {
            if (x >= pi_table.size())
                throw std::out_of_range(
                    "pi_lookup: argument exceeds pi_table"
                );

            return pi_table[static_cast<size_t>(x)];
        }

        // phi(x, a): count integers <= x not divisible by
        // any of the first a primes.
        uint64_t phi(
            uint64_t x,
            uint32_t a,
            const std::vector<uint32_t>& primes,
            PhiCache& cache)
        {
            if (a == 0)
                return x;

            if (a == 1)
                return x - x / 2;

            const PhiKey key{x, a};

            if (a < CACHE_A_LIMIT)
            {
                auto it = cache.find(key);

                if (it != cache.end())
                    return it->second;
            }

            const uint64_t value =
                phi(x, a - 1, primes, cache)
                - phi(x / primes[a - 1], a - 1, primes, cache);

            if (a < CACHE_A_LIMIT)
                cache.emplace(key, value);

            return value;
        }

        uint64_t lehmer_pi(
            uint64_t x,
            const PrimeData& data,
            PhiCache& phi_cache,
            PiCache& pi_cache)
        {
            if (x < 2)
                return 0;

            if (x < data.pi_table.size())
                return pi_lookup(x, data.pi_table);

            auto cached = pi_cache.find(x);

            if (cached != pi_cache.end())
                return cached->second;

            const uint64_t fourth_root = iroot4(x);
            const uint64_t square_root = isqrt(x);
            const uint64_t cube_root = icbrt(x);

            const uint64_t a = pi_lookup(
                fourth_root, data.pi_table);
            const uint64_t b = pi_lookup(
                square_root, data.pi_table);
            const uint64_t c = pi_lookup(
                cube_root, data.pi_table);

            // Lehmer's main expression.
            int64_t sum = static_cast<int64_t>(
                phi(x, static_cast<uint32_t>(a),
                    data.primes, phi_cache));

            sum += static_cast<int64_t>(
                (b + a - 2) * (b - a + 1) / 2);

            // Subtract the P2 contribution and, where needed,
            // the P3 correction.
            for (uint64_t i = a; i < b; ++i)
            {
                const uint64_t w = x / data.primes[i];

                sum -= static_cast<int64_t>(
                    lehmer_pi(w, data, phi_cache, pi_cache));

                if (i < c)
                {
                    const uint64_t limit = pi_lookup(
                        isqrt(w), data.pi_table);

                    for (uint64_t j = i; j < limit; ++j)
                    {
                        const uint64_t quotient =
                            w / data.primes[j];

                        const uint64_t count = lehmer_pi(
                            quotient, data, phi_cache, pi_cache);

                        sum -= static_cast<int64_t>(count)
                             - static_cast<int64_t>(j);
                    }
                }
            }

            const uint64_t answer = static_cast<uint64_t>(sum);
            pi_cache.emplace(x, answer);

            return answer;
        }
    }

    uint64_t prime_count_lehmer(uint64_t n)
    {
        if (n < 2)
            return 0;

        if (n <= 1'000'000)
        {
            PrimeData data =
                build_prime_data(static_cast<uint32_t>(n));

            return pi_lookup(n, data.pi_table);
        }

        const uint64_t sqrt_n = isqrt(n);

        if (sqrt_n > MAX_SIEVE_LIMIT)
        {
            throw std::length_error(
                "prime_count_lehmer: sqrt(n) exceeds "
                "the configured sieve limit"
            );
        }

        PrimeData data =
            build_prime_data(static_cast<uint32_t>(sqrt_n));

        PhiCache phi_cache;
        PiCache pi_cache;

        return lehmer_pi(n, data, phi_cache, pi_cache);
    }
}
