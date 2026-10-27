// std::copy(_InputIter first, _InputIter last, _OutputIter result) -- STLport copy(first, last, result) over non-trivially-assigned pointers: forward through the inline __copy_ptrs to the out-of-line __copy with a random_access_iterator_tag and a null Distance* (const CMapInfo*, CMapInfo*)
// Leaf: ??$copy@PBVCMapInfo@@PAV1@@std@@YIPAVCMapInfo@@PBV1@0PAV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005190b0, 20 bytes, __fastcall (first in ecx, last in edx, result on the stack), `ret 4`.
// Identity (probable): its callee 0x005150E0 is bound std::__copy<const CMapInfo*, CMapInfo*, int> (the 72-byte copy that the landed vector<CMapInfo>::_M_range_insert<const CMapInfo*> 0x00519CD0 calls); three arguments (`ret 4`) = the copy() front end; no caller (unreferenced COMDAT).
// Callee: std::__copy<const CMapInfo*, CMapInfo*, int> 0x005150E0 (bound ??$__copy@PBVCMapInfo@@PAV1@H@std@@...).
// Shape (family L002): `push 0 ; lea eax,[esp+8] ; push eax ; push result ; call` -- the empty tag temporary is homed on the
//   result argument's own slot (its address is pushed, nothing is stored), ecx/edx pass through untouched.
#include <stddef.h>
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}
class CMapInfo;                                            // Ego_r.pdb, 72 bytes (not touched here)
namespace std {
template <class _RandomAccessIter, class _OutputIter, class _Distance>                 // 0x005150E0
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
}
}
template CMapInfo* __fastcall std::copy(const CMapInfo*, const CMapInfo*, CMapInfo*);