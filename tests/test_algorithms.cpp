
#include <cassert>

#include "primecount/algorithms/trial_division.hpp"
#include "primecount/algorithms/sieve.hpp"
#include "primecount/algorithms/segmented_sieve.hpp"
#include "primecount/algorithms/legendre.hpp"
#include "primecount/algorithms/lehmer.hpp"

int main() {
    // Test primality
    assert(!primecount::is_prime(1));
    assert(!primecount::is_prime(0));
    assert(primecount::is_prime(2));
    assert(primecount::is_prime(3));
    assert(!primecount::is_prime(9));
    assert(!primecount::is_prime(91));
    assert(primecount::is_prime(97));

    // Test trial-division prime counting
    assert(primecount::prime_count(0) == 0);
    assert(primecount::prime_count(1) == 0);
    assert(primecount::prime_count(10) == 4);
    assert(primecount::prime_count(100) == 25);

    // Test sieve prime counting
    assert(primecount::prime_count_sieve(0) == 0);
    assert(primecount::prime_count_sieve(1) == 0);
    assert(primecount::prime_count_sieve(10) == 4);
    assert(primecount::prime_count_sieve(100) == 25);
    
    // Test segmented sieve
    assert(primecount::prime_count_segmented(-1) == 0);
    assert(primecount::prime_count_segmented(0) == 0);
    assert(primecount::prime_count_segmented(1) == 0);
    assert(primecount::prime_count_segmented(2) == 1);
    assert(primecount::prime_count_segmented(3) == 2);
    assert(primecount::prime_count_segmented(10) == 4);
    assert(primecount::prime_count_segmented(100) == 25);
    assert(primecount::prime_count_segmented(1000) == 168);
    assert(primecount::prime_count_segmented(1000000) == 78498);


    // Test Legendre prime counting
    assert(primecount::prime_count_legendre(0) == 0);
    assert(primecount::prime_count_legendre(1) == 0);
    assert(primecount::prime_count_legendre(10) == 4);
    assert(primecount::prime_count_legendre(100) == 25);
    assert(primecount::prime_count_legendre(1000000) == 78498);
    assert(primecount::prime_count_legendre(100000000) == 5761455);

    // Test Lehmer prime counting
    assert(primecount::prime_count_lehmer(0) == 0);
    assert(primecount::prime_count_lehmer(1) == 0);
    assert(primecount::prime_count_lehmer(10) == 4);
    assert(primecount::prime_count_lehmer(100) == 25);
    assert(primecount::prime_count_lehmer(1000000) == 78498);
    assert(primecount::prime_count_lehmer(100000000) == 5761455);
    assert(primecount::prime_count_lehmer(1000000000) == 50847534);

    return 0;
}
