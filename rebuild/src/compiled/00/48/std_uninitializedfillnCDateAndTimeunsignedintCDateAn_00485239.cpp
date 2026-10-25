// std::uninitialized_fill_n<CDateAndTime*,unsigned int,CDateAndTime>(first, n, x) -- the public STLport entry: forward to
// __uninitialized_fill_n with the __false_type tag (CDateAndTime has a user constructor, so it is not POD)
// Leaf: ??$uninitialized_fill_n@PAVCDateAndTime@@IV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485239, 19 bytes, `ret 4` (/Gr: ecx = first and edx = n pass straight through;
// x on the stack).
// Identity: the only callee is std::__uninitialized_fill_n<CDateAndTime*,unsigned int,CDateAndTime> (0x0040DF00); the
// tag temporary lives in the high byte of the x slot (`lea eax,[ebp+0Bh]`). Its sibling caller of 0x0040DF00 is
// vector<CDateAndTime>::_M_fill_insert (0x0048608C). The old label vector<EWeaponClass>::_Umove<EWeaponClass*> and the
// old body (an allocator construct call) were wrong; EWeaponClass is an enum, never a 28-byte record.
// Shape: __false_type has an empty user constructor (as in the landed 0x0048608C model); without it VC7.1 value-
// initialises the temporary (`stosb`) and gives it a frame slot.
#pragma optimize("s",on)

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

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                               const __false_type&);   // 0x0040DF00 for CDateAndTime

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template CDateAndTime* __fastcall uninitialized_fill_n<CDateAndTime*, unsigned int, CDateAndTime>(CDateAndTime*,
    unsigned int, const CDateAndTime&);
}
