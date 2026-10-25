#pragma optimize("s",on)
// std::fill<CDateAndTime*,CDateAndTime>(first, last, value) -- assign value to every record of [first, last)
// Leaf: ??$fill@PAVCDateAndTime@@V1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048521A, 31 bytes, `ret 4` (/Gr: ecx = first, edx = last; value on the stack).
// Identity: STLport 4.x `fill`: a pointer `!=` loop with a 7-dword `rep movsd` per element. Only caller:
// vector<CDateAndTime>::_M_fill_insert (0x0048608C), reached from resize(n) (0x0048623F) which builds its value with
// CDateAndTime::CDateAndTime (0x009E9AB0); Ego_r.pdb CDateAndTime is 28 bytes = the 0x1C stride. The old label _Fill<CHeroMarriageInfo*,...> was the Dinkumware name with a 24-byte element.
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
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill<CDateAndTime*, CDateAndTime>(CDateAndTime*, CDateAndTime*, const CDateAndTime&);
}
