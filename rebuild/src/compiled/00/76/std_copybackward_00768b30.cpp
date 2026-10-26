// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (pair<unsigned long, C3DVector> elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAU?$pair@KVC3DVector@@@std@@PAU12@H@std@@YIPAU?$pair@KVC3DVector@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768B30, 68 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (`sar eax,4` count).
// Identity (proven): __copy_backward<pair<unsigned long, C3DVector>*, pair<unsigned long, C3DVector>*, int>; evidence: its one checked caller is the __linear_insert 0x0076AAE0 (unsigned 16-byte keys, `copy_backward(first, last, last + 1)` at 0x0076AAFB), reached through the __insertion_sort 0x0076AEC0 from the sort 0x0076C000 whose introsort loop 0x0076BE30 is the landed pair<unsigned long, C3DVector> instance (CParticleUpdateCCompareVector<unsigned long>; NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> keeps that map); the TU's other 16-byte map (CParticleUpdateParamDesc) sorts through 0x0076C1A0.
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
typedef std::pair<unsigned long, C3DVector> TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);