// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (const CHeightMapCell, CHeightMapCell) STLport random-access forward copy of 12-byte records: n = last - first, then `*result = *first` n times; returns the advanced result
// Leaf: ??$__copy@PBVCHeightMapCell@@PAV1@H@std@@YIPAVCHeightMapCell@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F6630, 74 bytes (+ int3 padding), __fastcall (first ecx, last edx; result, the
//   tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<const CHeightMapCell*, CHeightMapCell*, int>; evidence: its callers are vector<CHeightMapCell>::operator= 0x008F6A90 (twice) and the unreferenced __copy_ptrs 0x008F67A0; 0x008F6A90's caller CHeightMap::Resize 0x008F61A0 (Ego_r.pdb ?Resize@CHeightMap@@AAEXJJJJ@Z) assigns a local grid to this+0x10 = Ego_r.pdb CHeightMap::Cells (vector<CHeightMapCell>, 12-byte cells); operator= passes const_pointer sources.
// Label: the manifest's "_Copy_impl<CTCSmokeGenerator::CGenerationPos*,CTCSmokeGenerator::CGenerationPos*>" is propagated (family J112: five byte-identical 12-byte __copy bodies; retail has no
//   identical-code folding).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` verbatim (the
//   landed 0x0062FC20 body; the landed __copy_backward 0x00ADAB20 spelling): the signed count by the reciprocal divide, one
//   3-dword record copy per iteration (the implicit assignment); `__fastcall` on the template definition is the TU's /Gr.
#include "engine/CHeightMapCell.h"

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CHeightMapCell* __fastcall __copy<const CHeightMapCell*, CHeightMapCell*, int>(const CHeightMapCell*, const CHeightMapCell*, CHeightMapCell*, const random_access_iterator_tag&, int*);
}   // namespace std