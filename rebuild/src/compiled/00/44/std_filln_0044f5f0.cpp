// std::fill_n(_OutputIter __first, _Size __n, const _Tp& __val) -- STLport fill_n over a 4-byte element with a signed count (size-optimised TU): store the value into n elements, return the end
// Leaf: ??$fill_n@PAUCUnprovenWord_0044f5f0@@HU1@@std@@YIPAUCUnprovenWord_0044f5f0@@PAU1@HABU1@@Z
// Retail: Fable.exe @ 0x0044F5F0, 23 bytes, __fastcall (first in ecx, n in edx, &val on the stack, `ret 4`).
// Identity: STLport 4.6 `fill_n(first, n, val)`; the signed size test (`test edx,edx ; jle`) means _Size = int. Its only
// references are two unreferenced /Os forwarding wrappers in the packed run at 0x00450B49 (`push [esp+4] ; call ;
// ret 8`, the __uninitialized_fill_n(.., __true_type) shape) and 0x00451500 (`jmp`, uninitialized_fill_n), so the
// element type is unproven: CUnprovenWord_0044f5f0 (descriptive).
// Shape: /Os (`#pragma optimize("s", on)`; the callers sit in a packed, unaligned run): the &val stack slot is re-read
// into ecx every pass (`mov ecx,[esp+4] ; mov ecx,[ecx]`), no loop-alignment pad.
#pragma optimize("s", on)
struct CUnprovenWord_0044f5f0 {
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
template CUnprovenWord_0044f5f0* __fastcall fill_n<CUnprovenWord_0044f5f0*, int, CUnprovenWord_0044f5f0>(
    CUnprovenWord_0044f5f0*, int, const CUnprovenWord_0044f5f0&);
}