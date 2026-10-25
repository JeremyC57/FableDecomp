#pragma optimize("s",on)
// std::__copy_backward_ptrs<CDateAndTime*,CDateAndTime*>(first, last, result, const __false_type&) -- the non-trivial-
// assignment path: forward to the random-access __copy_backward with a distance pointer of 0
// Leaf: ??$__copy_backward_ptrs@PAVCDateAndTime@@PAV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048563D, 21 bytes, `ret 8` (/Gr: ecx = first and edx = last pass straight
// through; result and the __false_type tag on the stack).
// Identity: STLport 4.x `__copy_backward_ptrs(first, last, result, const __false_type&)` = `__copy_backward(first, last,
// result, random_access_iterator_tag(), (ptrdiff_t*)0)`: the only callee is
// __copy_backward<CDateAndTime*,CDateAndTime*,int> (0x00485372, whose other caller is vector<CDateAndTime>::_M_fill_insert (0x0048608C), reached from resize(n) (0x0048623F) which builds its value with
// CDateAndTime::CDateAndTime (0x009E9AB0); Ego_r.pdb CDateAndTime is 28 bytes = the 0x1C stride). No direct callers
// (vector<CDateAndTime>::_M_fill_insert inlines its own copy). The tag temporary lives in the high byte of the dead
// tag-reference slot (`lea eax,[ebp+0Bh]`). The old label "CForwarder_0048563d::Forward" was descriptive.
#pragma optimize("y", off)              // frame pointer kept in this TU
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
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*);    // 0x00485372 for CDateAndTime

template <class _RandomAccessIter, class _BidirectionalIter>
_BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CDateAndTime* __fastcall __copy_backward_ptrs<CDateAndTime*, CDateAndTime*>(CDateAndTime*, CDateAndTime*,
    CDateAndTime*, const __false_type&);
}
