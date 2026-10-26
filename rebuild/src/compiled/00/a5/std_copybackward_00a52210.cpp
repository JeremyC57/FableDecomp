// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CUnprovenElement16_00a52210 elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAVCUnprovenElement16_00a52210@@PAV1@H@std@@YIPAVCUnprovenElement16_00a52210@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A52210, 68 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (`sar eax,4` count).
// Identity (descriptive): __copy_backward<CUnprovenElement16_00a52210*, CUnprovenElement16_00a52210*, int>; evidence: DESCRIPTIVE -- its callers are an unlanded vector range-insert 0x00A55190 (`copy_backward` at 0x00A5521B; reached from C3DAnimationSequenceData::InitialiseFromChunk 0x00A4EFC0) and a 16-byte block copy 0x00A535F0; the 16-byte element of that animation-keyframe vector (CQuaternion or C4DVector, both 16 bytes in Ego_r.pdb C3DKeyframe) is not proven.
// Label: the manifest's "_Move_backward<std::pair<unsigned___int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<unsigned___int64,class_NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>" is propagated across family H024 (seven bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 16-byte element (the assignment is four dword moves through ebx).
class CUnprovenElement16_00a52210 {                    // descriptive: 16 bytes, trivially assignable
public:
    long Word[4];
};
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
typedef CUnprovenElement16_00a52210 TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);