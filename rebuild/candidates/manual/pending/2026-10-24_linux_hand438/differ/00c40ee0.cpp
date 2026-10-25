// _global::MillibelsToAmplitude(long millibels) -- 0 at or below -10000 mB (silence); otherwise 10^(mB / 2000) computed as 2^(mB * 0.0005 * log2(10)) with frndint / f2xm1 / fscale (an inline-asm block in retail)
// Grade: DIFFER(38v109) -- retail_asm candidate: hand-written x87 2^x block (frndint / fwait / fld1 / fscale / f2xm1 / fwait over locals) after the C prologue; the C++ reference body calls pow. 1 grade.
// Leaf: ?MillibelsToAmplitude@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C40EE0, 109 bytes, __cdecl, float in st(0).
// Library: libpk audio (no PDB/RTTI); descriptive name. Called once, from 0x00C412EE. Constants: 0x01264AF0 =
//   0.0005f, the immediate 0x40549A78 = log2(10) (3.3219280f), 1.0f stored into the dead argument slot, 0x0122DEDC =
//   0.0f. The manifest's "Math_ScaleByLog2" is propagated.
// Retail-asm candidate: `fld [log2_10] ; fmul [e] ; frndint ; fst [i] ; fwait ; fld1 ; fscale ; fxch ; fstp ;
//   fld [log2_10] ; fmul [e] ; fsub [i] ; f2xm1 ; fadd [one] ; fmulp ; fstp [result] ; fwait` is a hand-written
//   2^x block (the `wait`s are asm-only). This C++ reference body (pow) is NOT byte-exact.
extern "C" double __cdecl pow(double x, double y);

float __cdecl MillibelsToAmplitude(long millibels)
{
    if (millibels <= -10000)
        return 0.0f;
    float exponent = millibels * 0.0005f;
    return (float)pow(10.0, exponent);
}
