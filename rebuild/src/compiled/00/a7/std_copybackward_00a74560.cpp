// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (C2DLineF elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAVC2DLineF@@PAV1@H@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A74560, 68 bytes, /Gr __fastcall (first ecx, last edx; result, the tag reference and the
//   distance-type pointer on the stack), `ret 0Ch`.
// Identity (proven): __copy_backward<C2DLineF*, C2DLineF*, int>; evidence: its caller 0x00A74AC0 (vector<C2DLineF>'s insert, called by the landed CArray<C2DLineF>::DeallocateUnusedMemory 0x00A74EA0) also calls the landed __uninitialized_copy<C2DLineF> 0x00A13490.
// Shape (family K049, representative 0x0098C240): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;` over a trivially assignable 16-byte element (four dword moves through ebx) --
//   the landed H024 rows (e.g. 0x00768B30) verbatim.
class C2DVector { public: float X, Y; };
class C2DLineF {                                         // Ego_r.pdb, 16 bytes
public:
    C2DVector Point1;                                    // +0x00
    C2DVector Point2;                                    // +0x08
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
typedef C2DLineF TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);