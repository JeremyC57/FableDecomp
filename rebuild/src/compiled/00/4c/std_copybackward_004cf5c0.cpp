// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (pair<float, CSubtitledMessage::CCameraTag> elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@PAU12@H@std@@YIPAU?$pair@MVCCameraTag@CSubtitledMessage@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CF5C0, 73 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (count by `imul 92492493h ; sar edx,4`).
// Identity (proven): __copy_backward<pair<float, CSubtitledMessage::CCameraTag>*, pair<float, CSubtitledMessage::CCameraTag>*, int>; evidence: its call sites are the landed __linear_insert 0x004D1000 (`copy_backward(first, last, last + 1)` at 0x004D101E) and the landed heap helpers 0x004CFEB0 / 0x004D0660 / 0x004D0B50 of the sort whose landed __partial_sort 0x004D1C80 orders CSubtitledMessage::CameraTags (Ego_r.pdb CVectorMap<float, CSubtitledMessage::CCameraTag>: pair<float, CCameraTag>, 4 + 24 bytes).
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is propagated across family H040 (six bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 28-byte element (the assignment is `rep movsd` with ecx = 7).
class CSubtitledMessage {
public:
    class CCameraTag {                                  // Ego_r.pdb, 24 bytes
    public:
        long Word[6];
    };
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
typedef std::pair<float, CSubtitledMessage::CCameraTag> TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);