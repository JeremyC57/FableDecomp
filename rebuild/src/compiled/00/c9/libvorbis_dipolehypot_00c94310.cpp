// libvorbis::dipole_hypot(float a, float b) -- signed "dipole" magnitude of a coupled M/A pair: sqrt(a^2 +- b^2) with the sign picked by the quadrant
// Leaf: dipole_hypot
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C94310, 213 bytes. libvorbis 1.0.1 lib/psy.c (static, used by
//   _vp_quantize_couple_memo for the lines below the point-stereo limit), compiled as C with /Gr: `__fastcall`
//   with both floats on the stack (`ret 8`). No callees; sqrt is the x87 intrinsic (`fsqrt`).
// Data: the double 0.0 @ 0x0122ED70 (the `0.` literal compares as double).
// Shape: the shipped source; VC evaluates both `a > 0.` and `b > 0.` up front (two fcomp/fnstsw before the
//   first branch) and tail-merges nothing -- each return keeps its own `fsqrt ; [fchs] ; ret 8`.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

extern "C" float __fastcall dipole_hypot(float a, float b)
{
    if (a > 0.) {
        if (b > 0.) return sqrt(a * a + b * b);
        if (a > -b) return sqrt(a * a - b * b);
        return -sqrt(b * b - a * a);
    }
    if (b < 0.) return -sqrt(a * a + b * b);
    if (-a > b) return -sqrt(a * a - b * b);
    return sqrt(b * b - a * a);
}