# primecount-cpp

A C++17 project exploring algorithms for computing the prime-counting function

$$
\pi(x)=\#\{p\le x:p\text{ is prime}\}.
$$

The project implements and benchmarks trial division, classical odd sieving, Legendre's, Lehmer's prime-counting algorithms.

## Algorithms

- **Trial division:** Tests candidate integers for primality by checking possible divisors.
- **Sieve of Eratosthenes:** Marks composite numbers to count primes up to a given limit.
- **Segmented sieve:** Processes ranges in blocks to reduce memory requirements compared with a full sieve.
- **Legendre's prime-counting algorithm:** Uses the partial sieve-counting function \(\phi(x,a)\) and a combinatorial identity to compute \(\pi(x)\).
- **Lehmer's prime-counting algorithm:** Uses recursive prime-counting identities and integer roots to count primes more efficiently for large inputs.

## Mathematical Background

The **prime-counting function** is

\[
\pi(x)=\sum_{n=2}^{\lfloor x\rfloor}\mathbf{1}_{\mathbb P}(n),
\]

where \(\mathbf{1}_{\mathbb P}(n)=1\) if \(n\) is prime and \(0\) otherwise. Thus, \(\pi(x)\) counts the primes not exceeding \(x\).

### 1. Trial Division

An integer \(n>1\) is prime if and only if it has no divisor \(d\) satisfying

\[
2\le d\le \lfloor\sqrt n\rfloor.
\]

After testing each candidate integer, the prime count is

\[
\pi(x)=\sum_{n=2}^{\lfloor x\rfloor}
\mathbf{1}_{\{n\text{ has no divisor }d,\,
2\le d\le\sqrt n\}}.
\]

Trial division is simple but expensive for large inputs because each candidate may require many divisibility checks.

### 2. Sieve of Eratosthenes

Initially mark every integer from \(2\) through \(N=\lfloor x\rfloor\) as potentially prime. For each unmarked integer \(p\le\sqrt N\), mark its multiples starting at \(p^2\) as composite.

The prime count is then

\[
\pi(N)=\sum_{n=2}^{N}
\mathbf{1}_{\{n\text{ remains unmarked}\}}.
\]

Starting at \(p^2\) is sufficient because every smaller multiple of \(p\) has a smaller prime factor and has already been marked.

### 3. Segmented Sieve

Partition the interval \([2,N]\) into smaller blocks. For a block \([L,R]\), use the primes up to \(\sqrt R\) to mark its composite numbers.

The final count is

\[
\pi(N)=\sum_{\text{blocks }[L,R]}
\sum_{n=L}^{R}
\mathbf{1}_{\{n\text{ is unmarked in its block}\}}.
\]

Unlike the ordinary sieve, the segmented sieve does not need to store a marker for every integer up to \(N\) simultaneously. It trades repeated block processing for lower memory usage.

### 4. Legendre's Prime-Counting Formula

Define

\[
\phi(x,a)=
\#\{n\le x:\text{ no prime }p\le p_a
\text{ divides }n\},
\]

where \(p_a\) denotes the \(a\)-th prime and \(\phi(x,0)=\lfloor x\rfloor\).

The function satisfies the recurrence

\[
\phi(x,a)=\phi(x,a-1)
-\phi\left(\left\lfloor\frac{x}{p_a}\right\rfloor,a-1\right).
\]

Let \(a=\pi(\sqrt x)\). Every integer counted by \(\phi(x,a)\), apart from \(1\), is either prime or a product of two primes both greater than \(\sqrt x\). The latter is impossible for integers not exceeding \(x\). Hence Legendre's formula is

\[
\boxed{\pi(x)=\phi(x,a)+a-1,\qquad
a=\pi(\sqrt x).}
\]

This formula reduces prime counting to evaluating \(\phi(x,a)\), which can itself be expensive when computed recursively.

### 5. Lehmer's Prime-Counting Algorithm

Lehmer's method improves on direct evaluation of Legendre's formula by combining partial sieving, precomputed prime counts, integer roots, and recursive prime counting.

One common form of the Lehmer formula uses

\[
a=\pi(x^{1/4}),\qquad
b=\pi(\sqrt x),\qquad
c=\pi(x^{1/3}),
\]

with integer-valued roots and floor operations understood. It can be written as

\[
\begin{aligned}
\pi(x)={}&\phi(x,a)
+\frac{(b+a-2)(b-a+1)}{2}\\
&-\sum_{i=a+1}^{b}
\pi\left(\left\lfloor\frac{x}{p_i}\right\rfloor\right)\\
&-\sum_{i=a+1}^{c}
\sum_{j=i}^{\pi(\sqrt{x/p_i})}
\left[
\pi\left(\left\lfloor\frac{x}{p_i p_j}\right\rfloor\right)
-(j-1)
\right].
\end{aligned}
\]

Here \(p_i\) is the \(i\)-th prime. Implementations differ in indexing conventions, so the summation limits and offsets must be adjusted consistently.

Lehmer's method avoids sieving every integer up to \(x\). It reuses smaller prime-counting results and is therefore much more practical for large values of \(x\).

### Summary

| Method | Main idea | Main limitation |
|---|---|---|
| Trial division | Test candidate divisors | Many divisibility checks |
| Sieve of Eratosthenes | Mark multiples of primes | Stores a range-sized marker array |
| Segmented sieve | Sieve one block at a time | Repeated block processing |
| Legendre | Count integers surviving a partial sieve | Recursive \(\phi\) computation |
| Lehmer | Combine partial sieving and recursive prime counts | More complex implementation and caching |


## Build

Requirements: a C++17 compiler and CMake.

```bash
cmake -S . -B build
cmake --build build
```

## Usage

Run the default small-input comparison:

```bash
./build/primecount
```

Run Lehmer's algorithm for a specified input:

```bash
./build/primecount --lehmer 10000000000
```

Run the tests:

```bash
./test.sh
```

Run the benchmark suite:

```bash
./bench.sh
```

## Benchmark results

### Five-run median benchmark

The following results were measured on the same machine with five repetitions per algorithm.

| Input \(N\) | Algorithm | Median runtime |
|---:|---|---:|
| \(10^8\) | Sieve of Eratosthenes | 8.281 s |
| \(10^8\) | Segmented sieve | 5.114 s |
| \(10^8\) | Legendre | 0.802 s |
| \(10^8\) | Lehmer | 0.0215 s |
| \(10^9\) | Sieve of Eratosthenes | 79.262 s |
| \(10^9\) | Segmented sieve | 50.790 s |
| \(10^9\) | Legendre | 7.782 s |
| \(10^9\) | Lehmer | 0.118 s |

At \(N=10^9\), Lehmer's algorithm was approximately 66 times faster than Legendre's algorithm and 672 times faster than the standard sieve in this benchmark.

### Lehmer scalability

These are individual wall-clock measurements, not medians.

| Input \(N\) | \(\pi(N)\) | Wall-clock time |
|---:|---:|---:|
| \(10^8\) | 5,761,455 | 0.027 s |
| \(10^9\) | 50,847,534 | 0.137 s |
| \(10^{10}\) | 455,052,511 | 0.828 s |
| \(10^{11}\) | 4,118,054,813 | 3.670 s |

The \(10^{12}\) attempt was interrupted and is not a completed measurement. Timings depend on hardware, compiler settings, and system load.

## Benchmark environment

- **CPU:** 11th Gen Intel Core i3-1115G4 @ 3.00 GHz
- **Logical CPUs:** 4
- **RAM:** 8 GB (7.5 GiB available to Linux)
- **OS:** Arch Linux
- **Compiler:** GCC 16.2.1
- **CMake:** 4.4.3
- **Build type:** Release

## Correctness tests

The test suite checks known prime counts and primality cases for the implemented algorithms, including larger reference values for Legendre's and Lehmer's methods.

The current test suite passes with CMake/CTest.

## Project status

Implemented:
- Trial division
- Standard sieve of Eratosthenes
- Segmented sieve
- Legendre prime counting
- Lehmer prime counting
- Benchmark executable and test suite

Additional sieve algorithms and further optimizations are planned.

## License

No license has been specified yet.

