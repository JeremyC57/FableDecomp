// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CQuaternion elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAVCQuaternion@@PAV1@H@std@@YIPAVCQuaternion@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A52210, 68 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (`sar eax,4` count).
// Types (proven, agentFAM11, 2026-11-12): __copy_backward<CQuaternion*, CQuaternion*, int>. Sources: (1) retail: the landed real-typed
//   std::vector<CQuaternion>::_M_range_insert 0x00A55190 calls it (`copy_backward` at 0x00A5521B) and its relocation binds exactly this
//   spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry vector<CQuaternion> (Ego_r struct
//   vector<CQuaternion,std::allocator<CQuaternion> >), and CQuaternion is the 16-byte X,Y,Z,W element. Was the placeholder
//   CUnprovenElement16_00a52210 (descriptive); the element now comes from the shared engine/CQuaternion.h.
// Label: the manifest's "_Move_backward<std::pair<unsigned___int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<unsigned___int64,class_NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>" is propagated across family H024 (seven bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 16-byte element (the assignment is four dword moves through ebx).
#include "engine/CQuaternion.h"
namespace std {
struct random_access_iterator_tag {};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}
typedef CQuaternion TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);