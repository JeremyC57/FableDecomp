#pragma once
typedef unsigned int size_t;
extern "C" { void* memcpy(void*,const void*,size_t); void* memmove(void*,const void*,size_t); void* memset(void*,int,size_t); int memcmp(const void*,const void*,size_t);
void* memchr(const void*,int,size_t); size_t strlen(const char*); int strcmp(const char*,const char*); int strncmp(const char*,const char*,size_t);
char* strcpy(char*,const char*); char* strncpy(char*,const char*,size_t); char* strcat(char*,const char*); char* strncat(char*,const char*,size_t);
char* strchr(const char*,int); char* strrchr(const char*,int); char* strstr(const char*,const char*); int _stricmp(const char*,const char*);
int _strnicmp(const char*,const char*,size_t); int stricmp(const char*,const char*); char* _strdup(const char*); char* _strlwr(char*); char* _strupr(char*);
size_t wcslen(const wchar_t*); int wcscmp(const wchar_t*,const wchar_t*); wchar_t* wcscpy(wchar_t*,const wchar_t*); int _wcsicmp(const wchar_t*,const wchar_t*);
wchar_t* wcsncpy(wchar_t*,const wchar_t*,size_t); wchar_t* wcscat(wchar_t*,const wchar_t*); wchar_t* wcschr(const wchar_t*,wchar_t); }
