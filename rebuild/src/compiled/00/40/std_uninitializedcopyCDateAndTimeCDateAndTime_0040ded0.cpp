// std::__uninitialized_copy<CDateAndTime*,CDateAndTime*>(first, last, result, const __false_type&) -- copy-construct
// [first, last) into raw storage at result, return the end of the copies
// Leaf: ??$__uninitialized_copy@PAVCDateAndTime@@PAV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040DED0, 47 bytes, `ret 8` (/Gr: ecx = first, edx = last; result and the
// __false_type tag on the stack).
// Identity: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (the non-POD path: one
// placement construct per element, hence the null test on the target). Its only caller is
// vector<CDateAndTime>::_M_fill_insert (0x0048608C, calls at 0x004860EE / 0x0048613C), reached from
// resize(n, x) (0x004861F7) <- resize(n) (0x0048623F), which builds the fill value with CDateAndTime::CDateAndTime
// (0x009E9AB0, zeroes 7 dwords). Ego_r.pdb CDateAndTime is 28 bytes (Year, Month, Day, Hour, Minute, Second,
// Millisecond) = the 0x1C stride. The old label std::_Copy_impl<CActionInputControl*,...> was the Dinkumware name of
// an identical-folded 28-byte body.
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

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CDateAndTime* __fastcall __uninitialized_copy<CDateAndTime*, CDateAndTime*>(CDateAndTime*, CDateAndTime*,
    CDateAndTime*, const __false_type&);
}
