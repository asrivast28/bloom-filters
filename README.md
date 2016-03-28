#CS 6505 - Bloom Filters Project

## Requirement
GNU compiler collection. Specifically, g++ with support for C++11.

## Compilation
Source can be compiled to get an executable using the provided Makefile, as

`make clean`
`make`

This will create a new executable in the directory called `bloom_filters`.

## Execution
The simulator can be executed as:
`bloom_filters <n> <m> <seed> <t>`
where
<n>    : table size of Bloom filter
<m>    : maximum number of items inserted into Bloom filter
<t>    : number of tests to be run for determining the false positive rate
<seed> : seed for the random number generator
