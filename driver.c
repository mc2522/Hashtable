/// file: driver.c
/// author: alp@cs.rit.edu
///  purpose: A driver for the CSCI243 hash table assignment.

#include "hashtable.h"

#include <assert.h> // assert
#include <stdint.h> // uint64_t
#include <stdio.h>
#include <stdlib.h> // rand
#include <time.h> // clock, CLOCKS_PER_SEC

// The maximum length of a key for THIS DRIVER ONLY.
// The general implementation should support arbitrary length keys!
// If you do not support arbitrary length keys, then you should lose points.
static const int max_key = 11; // strlen("4294967295") + 1;

// Generate random key/value pairs, and check they are set correctly.
// @param max_num -- the largest number to for a key
// @param trials -- the number of trials
static void checkit(int max_num, int trials) {
    char key[max_key];
    hash_t table = create_table(10);
    for (int i = 0; i < trials; i++) {
        int sample = rand() % max_num;
        sprintf(key, "%d", sample);
        set(table, key, sample);
    }
    unsigned int failures = 0;
    for (int i = 0; i < max_num; i++) {
        sprintf(key, "%d", i);
	int value = get(table, key, i);
        if(value != i) {
	    fprintf(stderr, "error: key/val mismatch for %s - got %d\n",key, value);
	    failures++;
	}
    }
    free_table(table);
    if (failures == 0) // ignoring the possibility of overflow
	printf("Sucessfully stored %d key/value pairs without"
	       " loss of information.\n", trials);
    else
	printf("Failed on %u of %d trials (%.2f).\n", failures, trials,
	       failures/(float)trials);
}

// Generate random key/value pairs and time the operation.
// @param max_num -- the largest number to for a key
// @param trials -- the number of trials
void timeit(int max_num, int trials) {
    char key[max_key];    
    hash_t table = create_table(10);
    uint64_t tics = 0;
    for (int i = 0; i < trials; i++) {
	int sample = rand() % max_num;
	sprintf(key, "%d", sample);
	uint64_t t1 = clock();
	set(table,key, 1 + get(table, key, 0));
	tics += clock() - t1;
    }
    free_table(table);
    printf("Test took %.2f seconds.\n", (float)tics/(float)CLOCKS_PER_SEC);

}

int main(int argc, char **argv) {
    // Default values
    int max_num = 100;
    int trials = 1000;

    if (argc == 3) {
	max_num = strtol(argv[1], NULL, 10);
	trials = strtol(argv[2], NULL, 10);
    } else if (argc != 1) {
	fprintf(stderr, "unexpected arguments.\n");
	fprintf(stderr, "usage: %s [max_num trials]\n", argv[0]);
	exit(-1);
    }
    checkit(max_num, trials);
    timeit(max_num, trials);
    return 0;
}
