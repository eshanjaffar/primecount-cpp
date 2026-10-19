#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <cstdint>

#include "primecount/algorithms/trial_division.hpp"
#include "primecount/algorithms/sieve.hpp"
#include "primecount/algorithms/segmented_sieve.hpp"
#include "primecount/algorithms/legendre.hpp"
#include "primecount/algorithms/lehmer.hpp"

using Clock = std::chrono::steady_clock;

template <typename Function>
double measure_ms(Function function, uint64_t N) {
    auto start = Clock::now();

    auto result = function(N);

    auto end = Clock::now();

    // Keep the computed result observable.
    (void)result;

    return std::chrono::duration<double, std::milli>(
        end - start
    ).count();
}

template <typename Function>
std::vector<double> benchmark(Function function, uint64_t N, int repetitions) {
    std::vector<double> times;

    for (int i = 0; i < repetitions; ++i) {
        times.push_back(measure_ms(function, N));
    }

    return times;
}

double median(std::vector<double> values) {
    std::sort(values.begin(), values.end());

    const std::size_t n = values.size();

    if (n % 2 == 1) {
        return values[n / 2];
    }

    return (values[n / 2 - 1] + values[n / 2]) / 2.0;
}

int main() {
    const uint64_t N = 1'000'000'000;
    const uint64_t expected_result = 50'847'534;
    const int repetitions = 5;

    // Verify correctness before benchmarking.
    // const long long trial_result = primecount::prime_count(N);
    const long long sieve_result = primecount::prime_count_sieve(N);
    const long long ssieve_result = primecount::prime_count_segmented(N);
    const uint64_t legendre_result = primecount::prime_count_legendre(N);
    const uint64_t lehmer_result = primecount::prime_count_lehmer(N);

    // checking correcteness of trial_division method (implemnted during initial part of the project)
    /*if (trial_result != sieve_result) {
        std::cerr << "ERROR: algorithms returned different results!\n";
        std::cerr << "Trial division: " << trial_result << '\n';
        std::cerr << "Sieve: " << sieve_result << '\n';
        return 1;
    }*/

    // checking correctness of simple odd sieve
    if (sieve_result != expected_result) {
        std::cerr << "ERROR: unexpected prime count: "
                  << sieve_result << '\n';
        return 1;
    }

    // checking correctness of segmented sieve
    if (ssieve_result != expected_result){
        std::cerr << "ERROR: Segmented Sieve returned an incorrect result: "
                  << ssieve_result << "\n";
        return 1;
    }

    // checking correctness of legendre sieve
    if (legendre_result != expected_result) {
        std::cerr << "ERROR: Legendre returned an incorrect result!\n";
        std::cerr << "Legendre: " << legendre_result << '\n';
        return 1;
    }

    if (lehmer_result != expected_result)
    {
        std::cerr << "ERROR: Lehmer sieve returned an incorrect result!\n";
        std::cerr << "Lehmer: " << lehmer_result << '\n';
        return 1;
    }

    std::cout << "Correctness check passed.\n";
    std::cout << "pi(" << N << ") = " << sieve_result << '\n';
    std::cout << "Repetitions: " << repetitions << "\n\n";

    // Trial division benchmark (implemnted during initial part of project)
    /*const auto trial_times = benchmark(
        primecount::prime_count, N, repetitions
    );*/

    const auto sieve_times = benchmark(
        primecount::prime_count_sieve, N, repetitions
    );

    const auto ssieve_times = benchmark(
        primecount::prime_count_segmented, N, repetitions
    );

    const auto legendre_times = benchmark(
        primecount::prime_count_legendre, N, repetitions
    );

    const auto lehmer_times = benchmark(
        primecount::prime_count_lehmer, N, repetitions
    );

    //const double trial_median = median(trial_times);
    const double legendre_median = median(legendre_times);
    const double sieve_median = median(sieve_times);
    const double ssieve_median = median(ssieve_times);
    const double lehmer_median = median(lehmer_times);

    /*
    std::cout << "Trial division median: "
              << trial_median << " ms\n";*/

    std::cout << "Sieve median:           "
              << sieve_median << " ms\n";

    std::cout << "Segmented Sieve median:        "
              << ssieve_median << " ms\n";

    std::cout << "Legendre median:        "
              << legendre_median << " ms\n";

    std::cout << "Lehmer median:           "
              << lehmer_median << " ms\n";

    std::cout << "Speedup:                "
              << legendre_median / lehmer_median << "x\n";

    return 0;
}