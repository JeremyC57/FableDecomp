// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (KnickKnack elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAUKnickKnack@@PAU1@H@std@@YIPAUKnickKnack@@PAU1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D9790, 68 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (`sar eax,4` count).
// Identity (proven): __copy_backward<KnickKnack*, KnickKnack*, int>; evidence: its caller 0x007D9D70 (`copy_backward` at 0x007D9DF9) is vector<KnickKnack>::_M_fill_insert, the callee of the landed vector<KnickKnack>::resize(n) 0x007D9FC0 (Ego_r.pdb CTCTavernGameSpotTheAddition::KnickKnacks; struct KnickKnack = C3DVector Pos + CThingObject* Thing, 16 bytes).
// Label: the manifest's "_Move_backward<std::pair<unsigned___int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<unsigned___int64,class_NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>" is propagated across family H024 (seven bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 16-byte element (the assignment is four dword moves through ebx).
class C3DVector {                                       // 12 bytes
public:
    float X, Y, Z;
    C3DVector() { }
};
class CThingObject;
struct KnickKnack {                                     // Ego_r.pdb, 16 bytes
    C3DVector     Pos;                                  // +0x0
    CThingObject* Thing;                                // +0xC
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
typedef KnickKnack TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);