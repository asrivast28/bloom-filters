#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <random>
#include <set>
#include <vector>


/**
 * @brief  Class for generating linear congrunential hash values.
 */
class LinearCongruentialHash {
public:
  LinearCongruentialHash()
    : m_n(), m_a(), m_b()
  { }

  LinearCongruentialHash(const size_t n, const size_t a, const size_t b)
    : m_n(n), m_a(a), m_b(b)
  { }

  size_t
  operator()(const size_t x) const
  {
    return (m_a * x + m_b) % m_n;
  }

private:
  size_t m_n;
  size_t m_a;
  size_t m_b;
};

/**
 * @brief  Class for generating k different hashes for Bloom filters.
 */
class HashFunction {
public:
  HashFunction(const size_t n, const size_t k, std::mt19937_64& generator)
    : m_hasher(k)
  {
    for (size_t i = 0; i < k; ++i) {
      std::uniform_int_distribution<size_t> distribution(1, n-1);
      size_t a = distribution(generator);
      size_t b = distribution(generator);
      m_hasher[i] = LinearCongruentialHash(n, a, b);
    }
  }

  std::vector<size_t>
  operator()(const size_t x) const
  {
    std::vector<size_t> hashes(m_hasher.size());
    for (size_t i = 0; i < m_hasher.size(); ++i) {
      hashes[i] = m_hasher[i](x);
    }
    return hashes;
  }

private:
  std::vector<LinearCongruentialHash> m_hasher;
};

/**
 * @brief  Class which implements Bloom filters.
 */
class BloomFilter {
public:
  BloomFilter(const size_t n, const size_t k, std::mt19937_64& generator)
    : m_hasher(n, k, generator), m_table(n, false)
  { }

  /**
   * @brief  Adds an element to the Bloom filter.
   *
   * @param x  Element to be added.
   */
  void
  add(const size_t x)
  {
    std::vector<size_t> positions(m_hasher(x));
    for (std::vector<size_t>::const_iterator pos = positions.begin(); pos != positions.end(); ++pos) {
      m_table[*pos] = true;
    }
  }

  /**
   * @brief  Checks for the element in the Bloom filter.
   *
   * @param x  Element to be checked in the Bloom filter.
   *
   * @return  true if the element exists in the Bloom filter, else returns false.
   */
  bool
  find(const size_t x) const
  {
    std::vector<size_t> positions(m_hasher(x));
    for (std::vector<size_t>::const_iterator pos = positions.begin(); pos != positions.end(); ++pos) {
      if (!m_table[*pos]) {
        return false;
      }
    }
    return true;
  }

private:
  const HashFunction m_hasher;
  std::vector<bool> m_table;
};

/**
 * @brief  Prints usage information for the generated executable.
 */
void
printUsage(
)
{
  std::cerr << "usage: bloom_filters n m" << std::endl;
  std::cerr << "Options and arguments:" << std::endl;
  std::cerr << "n    : table size of Bloom filter" << std::endl;
  std::cerr << "m    : maximum number of items inserted into Bloom filter" << std::endl;
}

/**
 * @brief  Calculates false positive rate for the given instance of Bloom filter.
 *
 * @param bf         Instance of the Bloom filter.
 * @param generator  Generator for generating random elements for the Bloom filter.
 * @param m          Maximum number of elements that can be inserted in the filter.
 * @param numTests   Number of tests to be run for determining the false positive rate.
 *
 * @return  Calculated false positive rate of the Bloom filter.
 */
double
calculateFalsePositiveRate(
  BloomFilter& bf,
  std::mt19937_64& generator,
  const size_t m,
  const size_t numTests
)
{
  double fpr = 0.0;
  std::set<size_t> added;

  std::uniform_int_distribution<size_t> distribution_add;
  for (size_t i = 0; i < m; ++i) {
    size_t element = distribution_add(generator);
    bf.add(element);
    added.insert(element);
  }

  size_t falsePositives = 0;
  std::uniform_int_distribution<size_t> distribution_find;
  for (size_t i = 0; i < numTests; ++i) {
    size_t element = distribution_find(generator);
    if (bf.find(element) && (added.find(element) == added.end())) {
      ++falsePositives;
    }
  }

  fpr = static_cast<double>(falsePositives) / numTests;

  return fpr;
}

/**
 * @brief  Main function for execution.
 *
 * @param argc  Number of arguments passed to the executable.
 * @param argv  Character pointer array containing all the arguments.
 *
 * @return  1 in case of any errors, else return 0.
 */
int
main(
  const int argc,
  const char** const argv
)
{
  // table size of the Bloom filter
  size_t n = 0;
  // capacity of the Bloom filter
  size_t m = 0;
  // number of tests to run for false +ve
  size_t numTests = 0;
  // seed for the random number generator
  size_t seed = 0;
  if (argc == 3) {
    // first and second argument must be n and m, respectively
    try {
      n = std::stoul(argv[1]);
      m = std::stoul(argv[2]);
      numTests = n;
      seed = static_cast<size_t>(time(NULL));
    }
    catch (std::exception& e) {
      std::cerr << "Invalid argument(s)" << std::endl;
      printUsage();
      return 1;
    }
  }
  else {
      std::cerr << "Invalid argument(s)" << std::endl;
      printUsage();
      return 1;
  }
  double c = static_cast<double>(n) / m;
  // number of hash functions in the Bloom filter
  size_t k = static_cast<size_t>(std::ceil(c * std::log(2)));

  std::mt19937_64 generator(seed);
  BloomFilter bf(n, k, generator);
  double fpr = calculateFalsePositiveRate(bf, generator, m, numTests);
  std::cout << "Calculated false positive rate is: " << fpr << std::endl;

  return 0;
}
