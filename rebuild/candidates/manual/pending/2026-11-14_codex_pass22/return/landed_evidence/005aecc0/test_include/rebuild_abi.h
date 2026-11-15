#pragma once
// Test adapter: host pointer and long widths remain unchanged.
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
