// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CActionInputControl elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAVCActionInputControl@@PAV1@H@std@@YIPAVCActionInputControl@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040EB60, 73 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (count by `imul 92492493h ; sar edx,4`).
// Identity (proven): __copy_backward<CActionInputControl*, CActionInputControl*, int>; evidence: its call site 0x00411E6C is in vector<CActionInputControl>::_M_fill_insert 0x00411DB0, the callee of the landed vector<CActionInputControl>::resize(n, x) 0x004129F0 (reached from the landed GFSerialiseVectorBinaryIn<CActionInputControl> 0x00412C70, FableWin.pdb ??$GFSerialiseVectorBinaryIn@VCActionInputControl@@); Ego_r.pdb CActionInputControl is 28 bytes.
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is propagated across family H040 (six bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 28-byte element (the assignment is `rep movsd` with ecx = 7).
class CActionInputControl {                             // Ego_r.pdb, 28 bytes
public:
    long Word[7];
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
typedef CActionInputControl TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);