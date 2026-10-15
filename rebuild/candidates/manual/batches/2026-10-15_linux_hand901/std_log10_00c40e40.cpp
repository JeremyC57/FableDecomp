// std::log10(float x) -- the single-precision log10 overload, emitted out of line (its address is taken)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40E40, length 11 bytes (16 with padding); no direct call. Part of
// the catalogue row 00c40e20 (skipped ledger, chunk 901) with std::atan2(float, float) at 0x00C40E30 and
// std::pow(float, float) at 0x00C40E50.
// Shape: the intrinsic (`#pragma intrinsic(log10)`): `fld [esp+4] ; fldlg2 ; fxch ; fyl2x ; ret`.
// Leaf: ?log10@std@@YAMM@Z
extern "C" double __cdecl log10(double);
#pragma intrinsic(log10)

namespace std {

float log10(float x)
{
    return (float)::log10((double)x);
}

}
