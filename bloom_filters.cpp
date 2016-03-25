#include <cmath>
#include <iostream>
#include <string>
#include <random>
#include <vector>


/**
 * @brief  Class for generating linear congrunential hash values.
 */
class LinearCongruentialHash {
public:
  LinearCongruentialHash()
    : m_n(),  m_a(), m_b()
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
  HashFunction(const size_t k, const size_t n, std::mt19937_64& generator)
    : m_hasher(k)
  {
    std::uniform_int_distribution<size_t> distribution(0, n-1);
    for (size_t i = 0; i < k; ++i) {
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
  BloomFilter(const size_t k, const size_t n, std::mt19937_64& generator)
    : m_hasher(k, n, generator), m_table(n, false)
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
  check(const size_t x) const
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
  std::cerr << "usage: bloom_filters n m seed" << std::endl;
  std::cerr << "Options and arguments:" << std::endl;
  std::cerr << "n    : table size of Bloom filter" << std::endl;
  std::cerr << "m    : maximum number of items inserted into Bloom filter" << std::endl;
  std::cerr << "seed : seed for the random number generator" << std::endl;
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
  // seed for the random number generator
  size_t seed = 0;
  if (argc == 4) {
    // first and second argument must be n and c, respectively
    try {
      n = std::stoul(argv[1]);
      m = std::stoul(argv[2]);
      seed = std::stoul(argv[3]);
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
  for (int i = 0; i < argc; ++i) {
    std::cout << argv[i] << std::endl;
  }
  double c = static_cast<double>(n) / m;
  // number of hash functions in the Bloom filter
  size_t k = static_cast<size_t>(std::ceil(c * std::log(2)));
  std::cout << c << " " << k << std::endl;

  std::mt19937_64 generator(seed);
  BloomFilter bf(n, k, generator);
  return 0;
}
