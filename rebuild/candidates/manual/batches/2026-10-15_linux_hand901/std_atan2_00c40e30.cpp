// std::atan2(float y, float x) -- the single-precision atan2 overload, emitted out of line (its address is taken)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40E30, length 11 bytes (16 with padding); no direct call -- the body
// exists because its address is taken (a maths function table). Part of the catalogue row 00c40e20 (skipped ledger,
// chunk 901) with std::log10(float) at 0x00C40E40 and std::pow(float, float) at 0x00C40E50.
// Shape: the intrinsic (`#pragma intrinsic(atan2)`): `fld [esp+4] ; fld [esp+8] ; fpatan ; ret`, the float
// arguments promoted for free by the loads and the result left in st(0).
// Leaf: ?atan2@std@@YAMMM@Z
extern "C" double __cdecl atan2(double, double);
#pragma intrinsic(atan2)

namespace std {

float atan2(float y, float x)
{
    return (float)::atan2((double)y, (double)x);
}

}
