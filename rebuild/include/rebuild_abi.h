#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __thiscall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_static_assert_, __LINE__)[(expression) ? 1 : -1]
// FABLE_STATIC_ASSERT_AT(n, expression): the same assert with its typedef name pinned to fable_static_assert_<n>, whatever line it is on.
// The plain macro names its typedef after __LINE__, and VC7.1's stack-slot order in a few rows depends on those names (row 0x007E2A60:
// six comment lines added to a header it includes swap two of its slots), so moving an assert is not a neutral edit. After editing a
// header, tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py respells every assert that moved with the line number
// it had before (docs/pipeline/GOTCHAS.md, landing fz). Use the plain macro for new asserts.
#define FABLE_STATIC_ASSERT_AT(pinned_line, expression) \
    typedef char FABLE_JOIN(fable_static_assert_, pinned_line)[(expression) ? 1 : -1]
