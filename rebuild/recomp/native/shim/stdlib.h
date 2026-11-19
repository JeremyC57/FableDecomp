#pragma once
typedef unsigned int size_t;
extern "C" { void* malloc(size_t); void free(void*); void* realloc(void*,size_t); void* calloc(size_t,size_t); int atoi(const char*); long atol(const char*);
double atof(const char*); long strtol(const char*,char**,int); unsigned long strtoul(const char*,char**,int); double strtod(const char*,char**); int rand(void);
void srand(unsigned); int abs(int); long labs(long); void qsort(void*,size_t,size_t,int(*)(const void*,const void*)); void exit(int);
char* _itoa(int,char*,int); char* _ltoa(long,char*,int); void abort(void); }
