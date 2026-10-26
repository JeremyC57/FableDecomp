// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (pair<C3DVector, long> elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAU?$pair@VC3DVector@@J@std@@PAU12@H@std@@YIPAU?$pair@VC3DVector@@J@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806310, 68 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (`sar eax,4` count).
// Identity (proven): __copy_backward<pair<C3DVector, long>*, pair<C3DVector, long>*, int>; evidence: its caller 0x00808AA0 (`copy_backward` at 0x00808AE1 after a float key compare) is the __linear_insert of the landed __insertion_sort 0x00809590 over the first-person targeting (position, index) pairs pair<C3DVector, long> ordered by CCompareTargetedPositionsForFirstPersonTargeting.
// Label: the manifest's "_Move_backward<std::pair<unsigned___int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<unsigned___int64,class_NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>" is propagated across family H024 (seven bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 16-byte element (the assignment is four dword moves through ebx).
class C3DVector {                                       // 12 bytes
public:
    float X, Y, Z;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
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
typedef std::pair<C3DVector, long> TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);