// std::fill_n(_OutputIter __first, _Size __n, const _Tp& __val) -- STLport fill_n over a 4-byte element: store the value into n consecutive elements, return the end
// Leaf: ??$fill_n@PAUCUnprovenWord_0043f4f0@@IU1@@std@@YIPAUCUnprovenWord_0043f4f0@@PAU1@IABU1@@Z
// Retail: Fable.exe @ 0x0043F4F0, 30 bytes, __fastcall (first in ecx, n in edx, &val on the stack, `ret 4`).
// Identity: STLport 4.6 `fill_n(first, n, val)` -- byte-identical to the landed std::fill_n<unsigned long*, unsigned int,
// unsigned long> 0x0043E060 of the same TU (unsigned size: `test edx,edx ; jbe`; the value re-read every pass since it
// may alias the range; the `lea ecx,[ecx]` loop-alignment pad), another instantiation over a different 4-byte element.
// It has no caller in retail (gap-scan COMDAT), so the element type is unproven: CUnprovenWord_0043f4f0 (descriptive).
struct CUnprovenWord_0043f4f0 {
    long _M_unproven;
};

namespace std {
template <class _OutputIter, class _Size, class _Tp>
_OutputIter __fastcall fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template CUnprovenWord_0043f4f0* __fastcall fill_n<CUnprovenWord_0043f4f0*, unsigned int, CUnprovenWord_0043f4f0>(
    CUnprovenWord_0043f4f0*, unsigned int, const CUnprovenWord_0043f4f0&);
}