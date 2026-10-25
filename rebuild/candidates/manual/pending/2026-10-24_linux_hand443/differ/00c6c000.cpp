// COnePoleLowPass::SetLevel(long levelMb, float referenceFrequency) -- Q15 coefficients of a one-pole low-pass whose power gain at the reference frequency is r = 10^(level/1000): a = 32768 * ((1 - r*c) - sqrt(r * (2*(1 - c) - r*(1 - c*c)))) / (1 - r) with c = cos(2*pi*f/44100); feedback = trunc(a), input = 0x7fff - feedback (the history is cleared when the input weight is 0); level 0 disables the filter
// Grade: DIFFER(199v240) -- retail_asm candidate: an inline-asm 10^x block (frndint / fwait / fscale / f2xm1 over C locals) and a bare `fistp dword` truncation helper; ours calls pow + __ftol2 | 1 grade | next: land as retail_asm
// Leaf: ?SetLevel@COnePoleLowPass
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6C000, 240 bytes, `ret 8`.
// Module: libpk's software mixer (descriptive names; the manifest's "Audio_ComputeDecibelsToLinear" is propagated).
//   Callers: 0x00C54CA8, 0x00C54D55 (voice set-up). Float pool: 0.0005f, 2*pi/44100 = 1.4247587e-04f, 32768.0f.
// Retail-asm: 0x00C6C039..0x00C6C069 is an inline-assembly 10^x block reading the C locals log2(10) (an immediate 3.321928f
//   stored to [esp+8]), one (1.0f at [esp]) and x ([esp+4]): `fld ; fmul ; frndint ; fst n ; fwait ; fld1 ; fscale ;
//   fxch ; fstp ; fld ; fmul ; fsub n ; f2xm1 ; fadd one ; fmulp ; fstp r ; fwait` (VC7.1's own exp/pow expansion uses
//   fldl2e and no fwait). 0x00C6C0BA is libpk's inline-assembly truncation helper (`fsub [0.5f] ; fstp ; fld ; fistp dword`).
//   fcos / fsqrt are the /Oi intrinsics of cos / sqrt.
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default). VC7.1 C++ cannot emit that (`(long)x` calls
// __ftol2 and /QIfist emits `fistp qword`), so the body below is a C++ stand-in; at landing it becomes
// the minimal inline-assembly `fld value ; fistp result` body (grade retail_asm, like the landed ROUND of 0x00BE6790).
__forceinline long FloatToLong(float value)
{
    return (long)(value < 0.0f ? value - 0.5f : value + 0.5f);     // stand-in: round to nearest (fistp: ties to even)
}
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
// (retail: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`)
__forceinline long FloatToLongTruncate(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
#include <math.h>
#pragma intrinsic(cos, sqrt)
struct COnePoleLowPass {
    long m_feedback;                               // +0x00 Q15 weight of the previous output
    long m_input;                                  // +0x04 Q15 weight of the input (0x7fff - feedback)
    long m_history;                                // +0x08 previous output
    void SetLevel(long levelMb, float referenceFrequency);
};
void COnePoleLowPass::SetLevel(long levelMb, float referenceFrequency)
{
    if (levelMb == 0) {
        m_feedback = levelMb;
        m_history = levelMb;
        return;
    }
    float exponent = (float)(levelMb + levelMb) * 0.0005f;
    // Retail-asm: r = 2^(exponent * log2(10)) through frndint / f2xm1 / fscale (see the header)
    float r = (float)pow(10.0, (double)exponent);
    float c = (float)cos(referenceFrequency * 1.4247587e-04f);
    float a = ((1.0f - r * c) - (float)sqrt(r * ((1.0f - c) * 2.0f - (1.0f - c * c) * r))) * 32768.0f / (1.0f - r);
    long feedback = FloatToLongTruncate(a);
    m_feedback = feedback;
    m_input = 0x7fff - feedback;
    if (m_input == 0)
        m_history = 0;
}
