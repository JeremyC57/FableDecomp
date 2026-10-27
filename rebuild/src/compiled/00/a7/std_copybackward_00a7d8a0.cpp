// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (std::pair<unsigned __int64, CANavQuadTreeNode*> elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAU?$pair@_KPAVCANavQuadTreeNode@@@std@@PAU12@H@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7D8A0, 68 bytes, /Gr __fastcall (first ecx, last edx; result, the tag reference and the
//   distance-type pointer on the stack), `ret 0Ch`.
// Identity (proven): __copy_backward<std::pair<unsigned __int64, CANavQuadTreeNode*>*, std::pair<unsigned __int64, CANavQuadTreeNode*>*, int>; evidence: its caller the landed __linear_insert 0x00A81CC0 names this callee over the 16-byte pair<unsigned __int64, CANavQuadTreeNode*> CNavQuadTree switchable-node entries (copy_backward(first, last, last + 1)).
// Shape (family K049, representative 0x0098C240): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;` over a trivially assignable 16-byte element (four dword moves through ebx) --
//   the landed H024 rows (e.g. 0x00768B30) verbatim.
class CANavQuadTreeNode;
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
typedef std::pair<unsigned __int64, CANavQuadTreeNode*> TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);