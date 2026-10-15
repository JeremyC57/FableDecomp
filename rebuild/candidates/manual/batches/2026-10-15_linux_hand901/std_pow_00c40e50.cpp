// std::pow(float x, float y) -- the single-precision pow overload, emitted out of line (its address is taken)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40E50, length 14 bytes (16 with padding); no direct call. Part of
// the catalogue row 00c40e20 (skipped ledger, chunk 901) with std::atan2(float, float) at 0x00C40E30 and
// std::log10(float) at 0x00C40E40.
// The result passes through a named float local: returning the call directly lets the lane tail-jump to _CIpow
// where retail keeps `call ; ret`.
// Shape: the intrinsic (`#pragma intrinsic(pow)`): both arguments pushed on the x87 stack and the CRT helper
// `_CIpow` (0x00BFEB40, an import thunk) called; the result stays in st(0).
// Leaf: ?pow@std@@YAMMM@Z
extern "C" double __cdecl pow(double, double);
#pragma intrinsic(pow)

namespace std {

float pow(float x, float y)
{
    float result = (float)::pow((double)x, (double)y);
    return result;
}

}
