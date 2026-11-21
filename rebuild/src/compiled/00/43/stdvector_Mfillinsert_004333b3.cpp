// std::vector<CCharString>::_M_fill_insert(CCharString* position, unsigned int n, const CCharString& x) -- STLport insert of n copies of a string: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXPAVCCharString@@IABV3@@Z
// Flags: /d2inlS34
// Retail: Fable.exe (TLC, VC7.1) @ 0x004333B3, 335 bytes, thiscall `ret 0Ch`; size TU with a frame pointer.
// Identity: stride 4 with CCharString's copy constructor 0x0099EC30, assignment 0x0099EFB0 and destructor 0x0099EAE0;
//   the growth path 0x00433530 is vector<CCharString>::_M_insert_overflow (GOTCHAS b48). The catalog label
//   "Std_Vector_InsertRange" is wrong (this is the fill insert, not a range insert).
// Callees: CCharString(const CCharString&) 0x0099EC30, CCharString::operator= 0x0099EFB0, uninitialized_fill_n(first, n, x)
//   0x00433504 (__fastcall), ~CCharString 0x0099EAE0 (x_copy), _M_insert_overflow(position, x, const __false_type&, n,
//   atend) 0x00433530.
// Shape: STLport 4.x `_M_fill_insert` verbatim (stl/_vector.c, the FABLE_RETAIL_STLP_VECTOR body), as 0x00472C2A: `x_copy` is
//   copy-constructed in the frame and destroyed at the end of the in-place branch; `__copy_backward` counts down with `sar 2`;
//   only the _M_insert_overflow call materialises its tag (`xor eax,eax ; stosb` at [ebp-1]).
// Name fix (NF212, 2026-11-21, q6332 / q6341): the local one-parameter vector<T> / CCharString / __false_type model is replaced
//   by the shared STLport 4.6 vector over fable_string.h. The explicit instantiation now defines the allocator spelling that the
//   three landed callers (0x0043332C, 0x0049B760, 0x0049B860) already bind, and the overflow call binds the protected IAE
//   _M_insert_overflow of the owner row 0x00433530 (declaration-only, as at HEAD). uninitialized_fill_n 0x00433504 stays
//   declaration-only (overlay opt-in FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY): the call now binds the template spelling
//   its owner row defines (the HEAD stand-in bound a non-template ?uninitialized_fill_n@std@@YI...); the visible noinline body
//   would emit a 41-byte copy against the owner's 38. FABLE_RETAIL_STLP_FALSE_TAG_POD gives the zeroed tag byte (`stosb`).
// Inline-budget: /d2inlS34 (the smallest N that expands __copy_backward_ptrs / fill in this size TU; 33 and below leave them
//   called out of line: DIFFER(320v337); no flag: DIFFER(300v337)). No forceinline opt-in: the three __uninitialized_copy loops
//   expand in line at S34 with the default `inline` body.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"

template void std::vector<CCharString>::_M_fill_insert(CCharString*, unsigned int, const CCharString&);
