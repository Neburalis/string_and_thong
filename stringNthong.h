#ifndef THRONG_H
#define THRONG_H

#include <stdio.h>

namespace mystr {

int put(const char * str);

char * chr(const char * str, const char c);

size_t len(const char * str);

char * copy(char * dst, const char * src);

char * ncopy(char * dst, const char * src, size_t count);

char * concat(char * dst, const char * src);

char * nconcat(char * dst, const char * src, size_t count);

char * dupeconcat(const char * first, const char * second);

char * fget(char * str, FILE *stream);

char * fnget(char * str, size_t count, FILE *stream);

char * dupe(const char * str);

char * getl(char * str, FILE * stream);

char * getnl(char * str, size_t count, FILE * stream);

int comp(const char * first, const char * second);

int ncomp(const char * first, const char * second, size_t size);

const char * err(int errcode);

char * mult(const char * src, size_t count);

}

#endif