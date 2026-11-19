#pragma once
typedef unsigned int size_t; typedef char* va_list; struct FILE;
extern "C" { int sprintf(char*,const char*,...); int _snprintf(char*,size_t,const char*,...); int printf(const char*,...); int vsprintf(char*,const char*,va_list);
int _vsnprintf(char*,size_t,const char*,va_list); int swprintf(wchar_t*,const wchar_t*,...); int sscanf(const char*,const char*,...); int puts(const char*); int _vsnwprintf(wchar_t*,size_t,const wchar_t*,va_list); }
