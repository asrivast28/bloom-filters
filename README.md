# Bloom Filters Simulator
In this project, Bloom filter was implemented in C++11. Mersenne Twister pseudo-random number generator engine was used for generating random numbers. The generator is programmed to use current time as seed for getting different results every time it is used.

Given *n* and *m*, the simulator works as follows. First, it instantiates the Bloom filter with *n* bits and *k* hash functions, where *k* is set to round(*n*/*m* ln 2). The hash functions are implemented as *h_i*(*x*) = (*a_i* * *x* + *b_i*) mod *n* for all *k*, where *a_i* and *b_i* are chosen from 1, ..., *n*-1 uniformly at random. Then, the simulator generates *m* random numbers and adds them to the Bloom filter.

The simulator then runs *n* tests for determining the actual false positive rate of the Bloom filter. For each test, a number is generated and its membership in the Bloom filter is tested. If the membership test returns true, then the element is searched for in the stored set of numbers. If the element is not found in the set, then the test is marked as a false positive. The probability of false positives is then calculated.


## Requirement
GNU compiler collection. Specifically, g++ with support for C++11.

## Compilation
Source can be compiled to get an executable using the provided Makefile, as

`make clean`

`make`

This will create a new executable in the directory called `bloom_filters`.

## Execution
The simulator can be executed as:

`bloom_filters <n> <m>`

where

`<n>`    : table size of Bloom filter

`<m>`    : maximum number of items inserted into Bloom filter
