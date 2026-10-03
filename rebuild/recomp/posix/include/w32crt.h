/* POSIX build: the Microsoft C runtime extensions the host uses, implemented in
   posix/msvcrt.cpp. Wide strings are UTF-16 (-fshort-wchar); the libc wide-string
   functions are replaced by 16-bit versions (posix/wchar16.cpp). */
#ifndef FABLE_W32CRT_H
#define FABLE_W32CRT_H
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct __locale_struct* _locale_t;
#ifndef _FSIZE_T_DEFINED
#define _FSIZE_T_DEFINED
typedef unsigned long _fsize_t;
#endif
struct _wfinddata32_t { unsigned attrib; int32_t time_create, time_access, time_write; uint32_t size; wchar_t name[260]; };
struct _finddata32_t { unsigned attrib; int32_t time_create, time_access, time_write; uint32_t size; char name[260]; };
#define _A_NORMAL 0x00
#define _A_RDONLY 0x01
#define _A_HIDDEN 0x02
#define _A_SYSTEM 0x04
#define _A_SUBDIR 0x10
#define _A_ARCH 0x20

char* _strdup(const char*);
int _stricmp(const char*, const char*);
int _strnicmp(const char*, const char*, size_t);
char* _strlwr(char*);
char* _strupr(char*);
int _wcsicmp(const wchar_t*, const wchar_t*);
int _wcsnicmp(const wchar_t*, const wchar_t*, size_t);
wchar_t* _wcsupr(wchar_t*);
wchar_t* _wcslwr(wchar_t*);
wchar_t* _wcsdup(const wchar_t*);
int _wtoi(const wchar_t*);
long _wtol(const wchar_t*);
double _wtof(const wchar_t*);
FILE* _wfopen(const wchar_t*, const wchar_t*);
int _wremove(const wchar_t*);
int _wrename(const wchar_t*, const wchar_t*);
int _wmkdir(const wchar_t*);
int _wrmdir(const wchar_t*);
int _wchdir(const wchar_t*);
wchar_t* _wgetcwd(wchar_t*, int);
int _mkdir(const char*);
int _chdir(const char*);
char* _getcwd(char*, int);
intptr_t _wfindfirst32(const wchar_t*, struct _wfinddata32_t*);
int _wfindnext32(intptr_t, struct _wfinddata32_t*);
intptr_t _findfirst32(const char*, struct _finddata32_t*);
int _findnext32(intptr_t, struct _finddata32_t*);
int _findclose(intptr_t);
void _wsplitpath(const wchar_t*, wchar_t*, wchar_t*, wchar_t*, wchar_t*);
void _wmakepath(wchar_t*, const wchar_t*, const wchar_t*, const wchar_t*, const wchar_t*);
void _splitpath(const char*, char*, char*, char*, char*);
void _makepath(char*, const char*, const char*, const char*, const char*);
int _snwprintf(wchar_t*, size_t, const wchar_t*, ...);
int _vsnwprintf(wchar_t*, size_t, const wchar_t*, va_list);
int _snprintf(char*, size_t, const char*, ...);
int _vsnprintf(char*, size_t, const char*, va_list);
char* _itoa(int, char*, int);
char* _ltoa(long, char*, int);
char* _ultoa(unsigned long, char*, int);
wchar_t* _itow(int, wchar_t*, int);
int64_t _atoi64(const char*);
int _access(const char*, int);
int _waccess(const wchar_t*, int);
unsigned char* _mbspbrk(const unsigned char*, const unsigned char*);
unsigned char* _mbsrchr(const unsigned char*, unsigned int);
unsigned char* _mbschr(const unsigned char*, unsigned int);
unsigned char* _mbsstr(const unsigned char*, const unsigned char*);
size_t _mbsspn(const unsigned char*, const unsigned char*);
size_t _mbscspn(const unsigned char*, const unsigned char*);
int _mbsnbcmp(const unsigned char*, const unsigned char*, size_t);
int _mbsicmp(const unsigned char*, const unsigned char*);
unsigned char* _mbsinc(const unsigned char*);
size_t _mbclen(const unsigned char*);
int _ismbcdigit(unsigned int);
int _ismbcspace(unsigned int);
int _ismbblead(unsigned int);
char* _strrev(char*);
int _getdrive(void);
int _chdrive(int);
#ifdef __cplusplus
}
#endif
#endif
