// libvorbis::round_hypot(float a, float b) -- signed circular magnitude of a coupled M/A pair: +-sqrt(a^2 + b^2) with the sign picked by the quadrant
// Leaf: round_hypot
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C943F0, 146 bytes. libvorbis 1.0.1 lib/psy.c (static, used by
//   _vp_quantize_couple_memo above the point-stereo limit), compiled as C with /Gr: `__fastcall` with both
//   floats on the stack (`ret 8`). No callees; sqrt is the x87 intrinsic.
// Data: the double 0.0 @ 0x0122ED70.
// Shape: the shipped source; every arm computes the same a*a+b*b (b*b+a*a commutes), so VC merges the
//   arms into two tails (`fsqrt ; fchs` and `fsqrt`) and shares one `jnp` into the positive tail.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

extern "C" float __fastcall round_hypot(float a, float b)
{
    if (a > 0.) {
        if (b > 0.) return sqrt(a * a + b * b);
        if (a > -b) return sqrt(a * a + b * b);
        return -sqrt(b * b + a * a);
    }
    if (b < 0.) return -sqrt(a * a + b * b);
    if (-a > b) return -sqrt(a * a + b * b);
    return sqrt(b * b + a * a);
}