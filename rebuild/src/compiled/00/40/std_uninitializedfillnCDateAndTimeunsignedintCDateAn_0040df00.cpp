// std::__uninitialized_fill_n<CDateAndTime*,unsigned int,CDateAndTime>(first, n, x, const __false_type&) --
// copy-construct n copies of x into raw storage at first, return the end of the copies
// Leaf: ??$__uninitialized_fill_n@PAVCDateAndTime@@IV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040DF00, 43 bytes, `ret 8` (/Gr: ecx = first, edx = n; x and the __false_type
// tag on the stack).
// Identity: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)` (one placement construct per
// element, hence the null test; `n > 0` on the unsigned size_type is the `test edx,edx ; jbe`). Callers:
// vector<CDateAndTime>::_M_fill_insert (0x0048608C, call 0x00486123) and the public 3-argument
// uninitialized_fill_n (0x00485239, call 0x00485243). The vector is reached from resize(n) (0x0048623F), which builds
// its value with CDateAndTime::CDateAndTime (0x009E9AB0); Ego_r.pdb CDateAndTime is 28 bytes = the 0x1C stride. The
// old label described this as Dinkumware `_Uninit_fill_n` with an unnamed element.
// Shape: the speed default aligns the loop head (`lea ecx,[ecx]` pad); the record copy is `rep movsd` x7.
#include <new>

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
struct __false_type {};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CDateAndTime* __fastcall __uninitialized_fill_n<CDateAndTime*, unsigned int, CDateAndTime>(CDateAndTime*,
    unsigned int, const CDateAndTime&, const __false_type&);
}
