#ifndef THRONG_H
#define THRONG_H

#include <stdio.h>

namespace mystr {

int put(const char * str);

char * find_char(const char * str, const char c);

size_t len(const char * str);

char * copy(char * dst, const char * src);

char * ncopy(char * dst, const char * src, size_t count);

char * concat(char * dst, const char * src);

char * nconcat(char * dst, const char * src, size_t count);

char * dupeconcat(const char * first, const char * second);

char * fget(char * str, FILE *stream);

char * fnget(char * str, size_t count, FILE *stream);

char * dupe(const char * str);

size_t getline(char ** ptr, size_t * len, FILE * stream);

int el_comp(const char * first, const char * second);

int comp(const char * first, const char * second);

int ncomp(const char * first, const char * second, size_t size);

const char * err(int errcode);

char * mult(const char * src, size_t count);

char * find_str(const char * haystack, const char * needle);

ssize_t count_needle_in_haystack(char * haystack, const size_t haystack_len, const char needle);

ssize_t replace_needle_in_haystack
    (char * haystack, const size_t haystack_len, const char src, const char dst);

void move_ptr_to_first_not_alpha_symbol(char ** ptr, int backword);

/*
this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c.
another version of this algorithm (now favored by bernstein) uses xor:
hash(i) = hash(i - 1) * 33 ^ str[i];
the magic of number 33 (why it works better than many other constants, prime or not)
has never been adequately explained.
*/
unsigned long hash(unsigned char *str);

/*
this algorithm was created for sdbm (a public-domain reimplementation of ndbm) database library.
it was found to do well in scrambling bits, causing better distribution of the keys and fewer splits.
it also happens to be a good general hashing function with good distribution.
the actual function is hash(i) = hash(i - 1) * 65599 + str[i];
what is included below is the faster version used in gawk.
[there is even a faster, duff-device version]
the magic prime constant 65599 (2^6 + 2^16 - 1) was picked out of thin air
while experimenting with many different constants. this is one of the algorithms
used in berkeley db (see sleepycat) and elsewhere.
*/
unsigned long sdbm(const char * str);

}

#endif