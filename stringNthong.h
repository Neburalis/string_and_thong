#ifndef THRONG_H
#define THRONG_H

#include <stdio.h>

int my_puts(const char * str);

char * my_strchr(const char * str, char c);

size_t my_strlen(const char * str);

char * my_strcpy(char * dst, const char * src);

char * my_strncpy(char * dst, const char * src, size_t count);

char * my_strcat(char * dst, const char * src);

char * my_strncat(char * dst, const char * src, size_t count);

char * my_fgets(char * str, FILE *stream);

char * my_fgetns(char * str, size_t count, FILE *stream);

char * my_strdup(const char * str);

char * my_getline(char * str, FILE * stream);

char * my_getnline(char * str, size_t count, FILE * stream);

int my_strcmp(const char * first, const char * second);

int my_strncmp(const char * first, const char * second, size_t size);

const char * my_strerror(int errcode);

#endif