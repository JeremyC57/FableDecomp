// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@PAV12@H@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC9EF0, 68 bytes, /Gr __fastcall (first ecx, last edx; result, the tag reference and the
//   distance-type pointer on the stack), `ret 0Ch`.
// Identity (proven): __copy_backward<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, int>; evidence: its caller the fill insert 0x00BCC080 also calls this element's __uninitialized_copy 0x00BCA370 and the landed vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::_M_insert_overflow 0x00BCBF80 (which names 0x00BCC080 its caller).
// Shape (family K049, representative 0x0098C240): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;` over a trivially assignable 16-byte element (four dword moves through ebx) --
//   the landed H024 rows (e.g. 0x00768B30) verbatim.
class C3DVector { public: float X, Y, Z; };
class CEngineInternalPrimitiveSpriteTrail {
public:
    class CSpriteTrailEntry {                            // Ego_r.pdb, 16 bytes
    public:
        C3DVector Position;                              // +0x00
        float Width;                                     // +0x0C
    };
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
typedef CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);