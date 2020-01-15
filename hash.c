/// file: hash.c
/// author: alp@cs.rit.edu
/// purpose: implements a simple hash function for strings

#include "hash.h"

/// Calculate a hash function.
/// @param key -- a null-teriminated character string
/// @return -- the hash value
unsigned int hash(char *key) {
    unsigned int sum = 0;
    while (*key != '\0')
	sum += *(key++);
    return sum;
}
