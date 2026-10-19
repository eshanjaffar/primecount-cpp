
cmake_minimum_required(VERSION 3.16)

project(primecount_cpp
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(primecount_lib
    src/algorithms/trial_division.cpp
    src/algorithms/sieve.cpp
    src/algorithms/segmented_sieve.cpp
    src/algorithms/legendre.cpp
    src/algorithms/lehmer.cpp
)

target_include_directories(primecount_lib PUBLIC include)

target_compile_options(primecount_lib PRIVATE
    -Wall -Wextra -Wpedantic
)

add_executable(primecount app/main.cpp)
target_link_libraries(primecount PRIVATE primecount_lib)

target_compile_options(primecount PRIVATE
    -Wall -Wextra -Wpedantic
)

add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark PRIVATE primecount_lib)

target_compile_options(benchmark PRIVATE
    -Wall -Wextra -Wpedantic
)


# automated testing
enable_testing()

add_executable(test_algorithms
    tests/test_algorithms.cpp
)

target_link_libraries(test_algorithms PRIVATE primecount_lib)

target_compile_options(test_algorithms PRIVATE
    -Wall -Wextra -Wpedantic
)

add_test(
    NAME algorithm_tests
    COMMAND test_algorithms
)
