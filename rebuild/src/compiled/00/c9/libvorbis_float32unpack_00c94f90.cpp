// libvorbis::_float32_unpack(long val) -- decode a Vorbis packed 32-bit float (21-bit mantissa, 10-bit biased exponent, sign bit) via ldexp
// Leaf: _float32_unpack
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C94F90, 52 bytes. libvorbis 1.0.1 lib/sharedbook.c, compiled as C with
//   /Gr: `__fastcall`, val in ecx, plain `ret` (the double from ldexp is returned in st0 unrounded).
// Callees: ldexp (MSVCR71 import [0x01440014]).
// Shape: the shipped source; the mantissa is `fild` of a `push ecx` frame word, the exponent
//   `(val & 0x7fe00000) >> 21` becomes `sar 15h ; and 3FFh`, and `exp - 20 - 768` is `add eax,-314h`.
extern "C" __declspec(dllimport) double __cdecl ldexp(double x, int exp);   // [0x01440014]

#define VQ_FEXP 10
#define VQ_FMAN 21
#define VQ_FEXP_BIAS 768

extern "C" float __fastcall _float32_unpack(long val)
{
    double mant = val & 0x1fffff;
    int    sign = val & 0x80000000;
    long   exp  = (val & 0x7fe00000L) >> VQ_FMAN;
    if (sign) mant = -mant;
    return ldexp(mant, exp - (VQ_FMAN - 1) - VQ_FEXP_BIAS);
}