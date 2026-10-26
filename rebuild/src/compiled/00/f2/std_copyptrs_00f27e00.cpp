// std::__copy_ptrs(const CHeroSoulsRoundDef* first, const CHeroSoulsRoundDef* last, CHeroSoulsRoundDef* result, const __false_type&) -- (CHeroSoulsRoundDef) STLport non-trivial range copy with __copy inline: assign each of the (last - first) / 0x38 records in turn (CDefClassBase::operator=, NumWaves, Waves), return the end of the destination
// Leaf: ??$__copy_ptrs@PBVCHeroSoulsRoundDef@@PAV1@@std@@YIPAVCHeroSoulsRoundDef@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F27E00, 92 bytes (+ int3 padding to 96), `__fastcall` (first in ecx, last in edx; result and the
//   empty __false_type reference on the stack), `ret 8`, returns the destination end.
// Identity (proven): std::__copy_ptrs<const CHeroSoulsRoundDef*, CHeroSoulsRoundDef*>; evidence: called from vector<CHeroSoulsRoundDef>::operator= 0x00F26C30 (both __copy_ptrs sites of STLport's assignment, over const_pointer ranges); the loop strides 0x38 = Ego_r sizeof CHeroSoulsRoundDef (60) - 4 (STLport vector), calls CDefClassBase::operator= on the record, copies the long at +0x28 and assigns the vector at +0x2C through 0x00F27BC0, which divides by 0x38 = retail sizeof CHeroSoulsWaveDef -- the only Ego_r {long @40, vector<X> @44} 60-byte class whose X has that retail size is CHeroSoulsRoundDef (NumWaves / Waves)
// Label: the manifest's "Vector_CopyElementsWithClanMemberAssignment" is propagated / descriptive (family J039: six __copy_ptrs bodies over 0x38-byte def records).
// Callees: CDefClassBase::operator= 0x00431F10 (??4CDefClassBase@@QAEAAV0@ABV0@@Z, the PDB spelling), std::vector<CHeroSoulsWaveDef >::operator=
//   0x00F27BC0 (the record's Waves member; its body divides by sizeof(CHeroSoulsWaveDef) = 0x38).
// Body: STLport 4.6 `return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);` with __copy's
//   `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }` inline; the
//   record's implicit operator= is inline too (base operator=, the long, the vector operator=). __copy needs __forceinline
//   (EXTRA_RULES; plain `inline` leaves it out of line: 20v92).
// Layout: Ego_r.pdb CHeroSoulsRoundDef (60; retail 0x38 with STLport's 12-byte vector): CDefClassBase (0x25, pack 1), NumWaves +0x28,
//   Waves (vector<CHeroSoulsWaveDef >) +0x2C. CDefClassBase comes from engine/CDefClassBase.h under a layout name (the header is
//   data-only; the PDB class declares the out-of-line operator=).
#include <stddef.h>
#define CDefClassBase CDefClassBasePdb
#include "engine/CDefClassBase.h"
#undef CDefClassBase
class CDefClassBase : public CDefClassBasePdb {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x00F27BC0
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                            _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
}   // namespace std
class CHeroSoulsWaveDef;
class CHeroSoulsRoundDef : public CDefClassBase {
public:
    long                            NumWaves;          // +0x28
    std::vector<CHeroSoulsWaveDef >     Waves;                // +0x2C
};
template CHeroSoulsRoundDef* __fastcall std::__copy_ptrs<const CHeroSoulsRoundDef*, CHeroSoulsRoundDef*>(const CHeroSoulsRoundDef*, const CHeroSoulsRoundDef*, CHeroSoulsRoundDef*, const std::__false_type&);