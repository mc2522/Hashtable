/// file: hashtable.h
/// author: alp@cs.rit.edu
/// purpose: An interface for a simple hash table.

/* This header is intended to be used with a chained
   hash-table implementation.  Correspondingly, there
   are two structure definitions -- one for the table
   itself, and another for the nodes of the individual
   chains. */

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stddef.h> // size_t

// structure for the nodes of the chains
struct node_s {
    char *key;
    int value;
    struct node_s *link;
};

/// This is the main structure for the overall table.
struct table_s {
    // This should be used as a pointer to a dynamically
    // allocated array of pointers to node structures.
    struct node_s **table;

    // This is for storing the maximum number of buckets/lists in the table.
    size_t bins;

    // This is for storing the current number of elements in the table
    size_t size;
};

/// A convenience declaration for referring to a pointer to a HT..
typedef struct table_s *hash_t;

/// Allocate a table with some initial empty bins.
/// @param bins -- the number of bins in the table (initally empty)
/// @return -- a pointer to a dynamically allocated hash table
hash_t create_table(int bins);

/// Set the value for a key in a given hash table.
/// @note -- if this is the first time setting this key, then the
//           table must make a dynamic copy of the string.  This
//           copy must be freed when the table is freed.
/// @note -- if the table exceeds a load factor of 1 after setting
//           the key/value pair, then this function should trigger
//           rehashing into a larger table.  It will then deallocate
//           the table field in the table_s structure, but it will
//           NOT free the table address in the table parameter.
/// @param table -- a pointer to a hash table
/// @param key -- a key
/// @param value -- a value
/// @return nothing
void set(hash_t table, char *key, int value);

/// Get the value for a key in a given hash table,
//    returning a default value if the key is not in the table.
/// @param table -- a pointer to a hash table
/// @param key -- a key
/// @param defval -- the default value for nonexistent keys
/// @return -- the value associated with the key (or the default value)
int get (hash_t table, char *key, int defval);


/// Free the table and all associated keys.
/// @param table -- a pointer to a hash table
/// @return nothing
void free_table(hash_t table);
#endif


