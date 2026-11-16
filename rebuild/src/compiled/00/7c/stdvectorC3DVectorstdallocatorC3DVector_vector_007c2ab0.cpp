// std::vector<C3DVector>::vector(size_type n) -- STLport's size constructor: start / finish / end-of-storage nulled, n 12-byte
// elements malloc'd (none for zero), then every element copy-constructed from one default C3DVector() temporary
// Leaf: ??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C2AB0, 115 bytes, `ret 4` (`sub esp,0Ch` holds the uninitialised T() temporary the fill copies).
// Identity: FableWin.pdb ??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@I@Z (0x018234C1, 0x02705D00); Ego_r.pdb has
//   the inline CArray<C3DVector>(unsigned int) that forwards to it (engine/CArray.h FABLE_RETAIL_CARRAY). Byte-proven through its
//   callers: 0x00B149D0 matches only when CArray(n) forwards to this vector(n) (3779 bytes; an out-of-line CArray(n) gives 3784), and
//   the landed 0x00B149D0 / 0x007BFE30 / 0x00B22B40 call it under this symbol (reloc_bindings).
// Respelled 2026-11-16 (was the placeholder CSTLportVector<C3DVector>::CSTLportVector, a local _Vector_base / alloc-proxy model with
//   a __forceinline fill helper): the body is now STLport's own, from the shared stlport/fable_containers.h --
//   `explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) { _M_finish = uninitialized_fill_n(_M_start, n, _Tp()); }` --
//   emitted by an explicit instantiation of that one member. VC7.1 expands _Vector_base, allocate (malloc 0x00BFEA0E) and the
//   __false_type uninitialized_fill_n loop inside it at the default inline budget (no flags).
// Callees: malloc @ 0x00BFEA0E (`__cdecl`).
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

template std::vector<C3DVector, std::allocator<C3DVector> >::vector(std::vector<C3DVector, std::allocator<C3DVector> >::size_type);
