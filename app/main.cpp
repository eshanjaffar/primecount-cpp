#include <iostream>
#include "primecount/algorithms/trial_division.hpp"
#include "primecount/algorithms/sieve.hpp"
#include "primecount/algorithms/segmented_sieve.hpp"
#include "primecount/algorithms/legendre.hpp"
#include "primecount/algorithms/lehmer.hpp"

int main(int argc, char *argv[])
{

  if (argc == 3)
  {
    if(std::string(argv[1]) == "--lehmer")
      {
          try
          {
              const uint64_t n = std::stoull(argv[2]);
              const uint64_t result = primecount::prime_count_lehmer(n);

              std::cout << "pi(" << n << ") = " << result << '\n';
          }
          catch (const std::exception& e)
          {
              std::cerr << "Error: " << e.what() << '\n';
              return 1;
          }
      }

      if(std::string(argv[1]) == "--legendre")
      {
          try
          {
              const uint64_t n = std::stoull(argv[2]);
              const uint64_t result = primecount::prime_count_legendre(n);

              std::cout << "pi(" << n << ") = " << result << '\n';
          }
          catch (const std::exception& e)
          {
              std::cerr << "Error: " << e.what() << '\n';
              return 1;
          }
      }

      if(std::string(argv[1]) == "--segmented_sieve")
      {
          try
          {
              const uint64_t n = std::stoull(argv[2]);
              const uint64_t result = primecount::prime_count_segmented(n);

              std::cout << "pi(" << n << ") = " << result << '\n';
          }
          catch (const std::exception& e)
          {
              std::cerr << "Error: " << e.what() << '\n';
              return 1;
          }
      }
        return 0;
    }

  if (argc != 1)
  {
    std::cerr << "Usage: " << argv[0]
              << " [--lehmer N]\n";
    return 1;
  }

  return 0;
}
