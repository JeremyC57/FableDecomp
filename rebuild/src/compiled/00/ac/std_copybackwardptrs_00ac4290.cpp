// std::__copy_backward_ptrs(_RandomAccessIter first, _RandomAccessIter last, _BidirectionalIter result, const __false_type&) -- (C3DVertexBlend2*) STLport backward copy for an 8-byte element with a non-trivial (__false_type) assignment: walk n = last - first elements down from the ends, `*--result = *--last`, and return the new start of the destination
// Leaf: ??$__copy_backward_ptrs@PAVC3DVertexBlend2@@PAV1@@std@@YIPAVC3DVertexBlend2@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC4290, 43 bytes (+ int3 padding), __fastcall (first ecx, last edx), `ret 8`; returns result - n in eax.
// Types (proven, agentFAM11, 2026-11-12): element C3DVertexBlend2. Sources: (1) retail: the landed real-typed vector<C3DVertexBlend2>::
//   _M_fill_insert 0x00AC42F0 calls it and its relocation binds exactly this spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb /
//   FableWin.pdb carry vector<C3DVertexBlend2> and C3DVertexBlend2 is 8 bytes (VertexIndex, BlendValue), the two-dword copy below. Was the
//   placeholder CUnprovenElement_00ac4290 (descriptive); the element now comes from the shared engine/C3DVertexBlend2.h.
//   Family F013 (15 bodies, one COMDAT per element type; 14 `ret 8` __copy_backward_ptrs + this family's `ret 4`
//   copy_backward 0x00B855C0): the manifest label "_Copy_impl<std::pair<float,EConversationAttitude>*,std::pair<float,enum_EConversationAttitude>*>" is
//   propagated across the byte twins (an Ego_r Dinkumware spelling; no row of the family is proven to hold that pair).
//   STLport 4.6 _algobase.h: the non-trivial-assignment `__copy_backward_ptrs` that vector::_M_fill_insert / insert call directly with their `_TrivialAss()` tag, the random-access `__copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined; `__fastcall` on the template definition.
// Shape: the element's compiler-generated assignment is an 8-byte block copy (two dwords through edx); the loop keeps
//   n in esi and addresses the source through the constant distance last - result (`mov ecx,edx ; sub ecx,eax`).
#include "engine/C3DVertexBlend2.h"
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter>
inline _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                          _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template C3DVertexBlend2* __fastcall __copy_backward_ptrs<C3DVertexBlend2*, C3DVertexBlend2*>(C3DVertexBlend2*, C3DVertexBlend2*, C3DVertexBlend2*, const __false_type&);
}