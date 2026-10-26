// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CPlane elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAVCPlane@@PAV1@H@std@@YIPAVCPlane@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB0C0, 68 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (`sar eax,4` count).
// Identity (proven): __copy_backward<CPlane*, CPlane*, int>; evidence: its caller 0x00AAB570 (`copy_backward` at 0x00AAB5F9) is vector<CPlane>::insert(pos, n, const CPlane&) per the landed C3DMeshFileConvexVolumeChunk::ReadFromFile 0x00AAAB10 (its Planes vector resized to the stored count).
// Label: the manifest's "_Move_backward<std::pair<unsigned___int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<unsigned___int64,class_NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>" is propagated across family H024 (seven bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 16-byte element (the assignment is four dword moves through ebx).
class CPlane {                                          // 16 bytes (normal + distance)
public:
    float Data[4];
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
typedef CPlane TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);