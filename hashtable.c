/*
 * File: hashtable.c
 * date: 3/5/19
 * @author Mike Cao 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "hash.h"

/* 
 * Allocate a table with some initial empty bins.
 * @param bins -- the number of bins in the table (initally empty)
 * @return -- a pointer to a dynamically allocated hash table
 */
hash_t create_table(int bins) {
	// allocate appropriate memory to the new table
	hash_t hTable = (hash_t)malloc(sizeof(struct table_s));
	// allocate the array of nodes the appropriate memory according to the amount of bins
	struct node_s **array = (struct node_s**)
                malloc(sizeof(struct node_s) * bins);
	// initialize everything in array to NULL
	for(int i = 0; i < bins; i++) 
		array[i] = NULL;
	// set array
	hTable->table = array;
	// assign the provided bins to the new table's number of bins
	hTable->bins = bins;
	// set the new table's size to 0 since there's nothing in it yet
	hTable->size = 0;
	// return the table	
	return hTable;
}

/*
 * Helper function to free_table
 * Frees the nodes, their keys, and the table array
 * @param table -- the pointer to the array of nodes
 * @param size -- the number of elements in the table array
 */
void helpFree(struct node_s** table, size_t size) {
	// holds the previous node temporarily
	struct node_s *previous;
	// hold the next node
	struct node_s *node;
        // go through each linked list in each index
	for(unsigned int i = 0; i < size; i++) {
		node = table[i];
		// free nodes in the linked lists
		if(node != NULL) {
			// initialize previous to NULL
			previous = NULL;
			while(node->link != NULL) {
				// assign previous to node to free it 
				previous = node;
				// go to the next node
				node = node->link;
				// free the previous key
				free(previous->key);
				// free the previous node
				free(previous);
			}
			// free the key
			free(node->key);
			// free the node
			free(node);
		}
	}
	// free the hash_t table
	free(table);
}

/*
 * Free the table and all associated keys.
 * @param table -- a pointer to a hash table
 * @return nothing
 */
void free_table(hash_t table) {
	// free the inside of the hash_t table
	helpFree(table->table, table->size);
	// free the hash_t table
	free(table);
}                 

/* Set the value for a key in a given hash table.
 * @note -- if this is the first time setting this key, then the
 *           table must make a dynamic copy of the string.  This
 *           copy must be freed when the table is freed.
 * @note -- if the table exceeds a load factor of 1 after setting
 *           the key/value pair, then this function should trigger
 *           rehashing into a larger table.  It will then deallocate
 *           the table field in the table_s structure, but it will
 *           NOT free the table address in the table parameter.
 * @param table -- a pointer to a hash table
 * @param key -- a key
 * @param value -- a value
 * @return nothing
 */
void set(hash_t table, char *key, int value) {
	// assign bin the index for the table index
	unsigned int bin = hash(key) % table->bins;
	// if 1, then the maximum capacity is reached, and must rehash
	size_t loadFactor;
	// the linked list
	struct node_s *node = table->table[bin];
	// allocate newPair the size of struct node_s
	struct node_s *newPair = (struct node_s*)malloc(sizeof(struct node_s));
	// allocate the key the appropriate memory
	newPair->key = malloc(sizeof(key) + 1);
	// copy the key to newPair
	strcpy(newPair->key, key);
	// set newPair->value to the provided value
	newPair->value = value;
	// set newPair link to NULL
	newPair->link = NULL;
	// check if the list is empty
	if(table->table[bin] == NULL) {
		// if empty, set newPair to the first spot in the list
		table->table[bin] = newPair;
		// increase size by one
		table->size++;
	} else {
		// if the next node is not NULL
		while(node->link != NULL) {
			// if the key is already in the list...
			if(strcmp(node->key, key) == 0) {
				// set the node value to the new value
				node->value = value;
				// free the provided key since it is unneeded
				free(newPair->key);
				// free the newPair since it is unneeded
				free(newPair);
				// break out of the function
				return;
			}
			// go to the next node
			node = node->link;
		}
		// set the next node to newPair if it reaches the last taken spot in the 
		// linked list
		node->link = newPair;
		// increment size by one
		table->size++;
	}
	// calculate loadFactor to see if the maximum capacity is reached
	loadFactor = table->size / table->bins;
	// if max capacity is reached...
	if(loadFactor == 1) {
		// holds the old array
		struct node_s** temp = table->table;
		// allocate the new array to twice the size
		struct node_s** ptr = (struct node_s**)
			malloc(sizeof(struct node_s) * table->bins * 2);
		// keep the old size
		size_t size = table->size;
		// double the number of bins
		table->bins *= 2;
		// reset the size since we're rehashing
		table->size = 0;
		// initialize everything to NULL
		for(unsigned int i = 0; i < table->bins; i++)
			ptr[i] = NULL;
		// point the old array to the new array
		table->table = ptr;
		// holds the current temp[i]
		struct node_s *old;
		// go through everything
		for(unsigned int i = 0; i < size; i++) {
			old = temp[i];
			// while temp[i] is not NULL...
			while(old != NULL) {
				// recursively rehash the new nodes
				set(table, temp[i]->key, temp[i]->value);
				// go to the next node in the old list
				old = old->link;
			}
		}
		// free everything in temp
		helpFree(temp, size);
	}
}

/* 
 * Get the value for a key in a given hash table,
 *   returning a default value if the key is not in the table.
 * @param table -- a pointer to a hash table
 * @param key -- a key
 * @param defval -- the default value for nonexistent keys
 * @return -- the value associated with the key (or the default value)
 */
int get(hash_t table, char *key, int defval) {
	// index of the table array
	unsigned int bin = hash(key) % table->bins;
	// linked list	
	struct node_s *node = table->table[bin];
	// check if the first pair's key matches
	if(node == NULL) 
		// if matching, return the value
		return defval;
	// keep going until there's nothing else in the linked list...
	while(node != NULL) {
		// if the key in the next pair matches...
		if(strcmp(node->key, key) == 0)
			//return the next pair's value
			return node->value;
		// if key doesn't match, go to the next pair in the linked list
		node = node->link;
	}
	// if it reaches this point...
	return defval;
}
