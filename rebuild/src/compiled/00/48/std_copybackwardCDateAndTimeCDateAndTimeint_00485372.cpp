#pragma optimize("s",on)
// std::__copy_backward<CDateAndTime*,CDateAndTime*,int>(first, last, result, const random_access_iterator_tag&, int*) --
// copy [first, last) backwards so it ends at result, return the start of the copies
// Leaf: ??$__copy_backward@PAVCDateAndTime@@PAV1@H@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485372, 51 bytes, `ret 0Ch` (/Gr: ecx = first, edx = last; result, the tag and
// the distance pointer on the stack).
// Identity: STLport 4.x random-access `__copy_backward`: a signed element count (`idiv` by 0x1C) and a counted
// countdown loop, each element a 7-dword `rep movsd`. Callers: vector<CDateAndTime>::_M_fill_insert (0x0048608C), reached from resize(n) (0x0048623F) which builds its value with
// CDateAndTime::CDateAndTime (0x009E9AB0); Ego_r.pdb CDateAndTime is 28 bytes = the 0x1C stride; and the out-of-line __copy_backward_ptrs
// (0x0048563D). The old label _Copy_backward<CHeroMarriageInfo*,...> was the Dinkumware name with a 24-byte element.
// Shape: result stays in its stack home and is decremented in place (`rep movsd` clobbers esi/edi/ecx).
class CDateAndTime {
public:
    long          Year;                                 // +0x00
    unsigned long Month;                                // +0x04
    unsigned long Day;                                  // +0x08
    unsigned long Hour;                                 // +0x0C
    unsigned long Minute;                               // +0x10
    unsigned long Second;                               // +0x14
    unsigned long Millisecond;                          // +0x18
    CDateAndTime();                                     // 0x009E9AB0
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

template CDateAndTime* __fastcall __copy_backward<CDateAndTime*, CDateAndTime*, int>(CDateAndTime*, CDateAndTime*,
    CDateAndTime*, const random_access_iterator_tag&, int*);
}
