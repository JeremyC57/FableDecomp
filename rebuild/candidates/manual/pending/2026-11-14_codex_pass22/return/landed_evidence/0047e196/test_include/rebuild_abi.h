#pragma once
// Fixture ABI adapter only: host pointers/long retain their real widths.
#ifdef _MSC_VER
#define FABLE_CDECL __cdecl
#define FABLE_STDCALL __stdcall
#define FABLE_FASTCALL __fastcall
#define FABLE_THISCALL __thiscall
#else
#define FABLE_CDECL
#define FABLE_STDCALL
#define FABLE_FASTCALL
#define FABLE_THISCALL
#define __int64 long long
#define __fastcall
#define __cdecl
#endif
typedef signed char fable_i8;typedef unsigned char fable_u8;
typedef signed short fable_i16;typedef unsigned short fable_u16;
typedef signed int fable_i32;typedef unsigned int fable_u32;
#define FABLE_STATIC_ASSERT(expression)
#define FABLE_STATIC_ASSERT_AT(n,expression)
