// CUnprovenPow10_00c42c30(float x) -- 10 to the power x by the x87 split: n = round(x * log2(10)), 2^n via fscale, 2^frac via f2xm1 + 1
// Grade: DIFFER(15v75) -- x87 10^x inline-assembly kernel (frndint/f2xm1/fscale): no C++ form
// Leaf: ?CUnprovenPow10_00c42c30@@YAMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42C30, 75 bytes, cdecl `float(float)`, the result returned in st(0).
// Identity (descriptive): one of six byte-identical copies (0x00C40E60, 0x00C42C30, 0x00C54A80, 0x00C5C0B0, 0x00C6BF60,
//   0x00C77690) in the audio library TUs; no call, jump or address reference reaches any of them.
// Not reconstructed: retail is an inline-assembler x87 kernel (frndint / f2xm1 / fscale with explicit fwait, the constants
//   log2(10) = 0x40549A78 and 1.0f staged in its own frame); VC7.1 C++ cannot emit those instructions, so this arithmetic
//   model compiles to a CRT pow call instead (the differ copy, never a landing candidate).
extern "C" double __cdecl pow(double x, double y);
float CUnprovenPow10_00c42c30(float x)
{
    return (float)pow(10.0, (double)x);
}
