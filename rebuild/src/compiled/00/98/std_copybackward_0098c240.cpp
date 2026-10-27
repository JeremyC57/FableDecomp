// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (C4DVector elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAVC4DVector@@PAV1@H@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098C240, 68 bytes, /Gr __fastcall (first ecx, last edx; result, the tag reference and the
//   distance-type pointer on the stack), `ret 0Ch`.
// Identity (proven): __copy_backward<C4DVector*, C4DVector*, int>; evidence: its callers are the landed vector<C4DVector>::_M_fill_insert 0x0098D360 (which names this callee) and the C4DVector vector code of the same TU (CShaderRenderManager's LightGlobals); bound ??$__copy_backward@PAVC4DVector@@....
// Shape (family K049, representative 0x0098C240): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;` over a trivially assignable 16-byte element (four dword moves through ebx) --
//   the landed H024 rows (e.g. 0x00768B30) verbatim.
class C4DVector {                                        // Ego_r.pdb, 16 bytes
public:
    float X, Y, Z, W;
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
typedef C4DVector TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);