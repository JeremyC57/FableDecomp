// COnePoleLowPass::SetLevel(long levelMb, float referenceFrequency) -- libpk audio: set a one-pole low-pass filter's Q15 feedback and input weights from an attenuation in millibels (10^(mB/1000) through the x87 2^x block) and a reference frequency; zero resets it
// Leaf: ?SetLevel@COnePoleLowPass
// Retail-asm: 0x00C6C039..0x00C6C069 `fld [esp+8] ; fmul [esp+4] ; frndint ; fst [esp+0Ch] ; wait ; fld1 ; fscale ; fxch st(1) ; fstp [esp+10h] ; fld [esp+8] ; fmul [esp+4] ; fsub [esp+0Ch] ; f2xm1 ; fadd [esp] ; fmulp st(1),st ; fstp [esp+18h] ; wait` -- libpk's inline-assembly 2^x block over named float locals (17 instructions, no branch: rasm1 family (e), the Pow10 helper 0x00C53B10 expanded in place with its result homed in the dead `levelMb` slot); frndint, fscale, f2xm1 and the `wait`s have no C++ spelling in VC7.1 (`pow` becomes a `_CIpow` jump: rasm1 probe). Then 0x00C6C0C4 `fld dword ptr [esp+18h]` ; 0x00C6C0C8 `fistp dword ptr [esp+1Ch]` -- the inline-assembly truncation helper (rasm1 family (a)): the coefficient less 0.5 stored to the same dead slot and reloaded for a bare dword fistp into the dead `referenceFrequency` slot, with the C++ `< 0 ? +1` fix-up. The zero test, the exponent, the cosine/square-root filter arithmetic and the three stores are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6C000, 240 bytes, __fastcall (this = the filter in ecx), `ret 8`. Constants:
//   0.0005f @ 0x01264AF0, 2*pi/44100 @ 0x012BED78, 1.0f @ 0x0122DED8, 32768.0f @ 0x0129EF98, 0.5f @ 0x0122F59C;
//   log2(10) = 0x40549A78 as an immediate. The catalog name "Audio_ComputeDecibelsToLinear" is descriptive.
#include <math.h>
#pragma intrinsic(cos, sqrt)
// libpk's inline-assembly 10^x helper (0x00C53B10 is its standalone copy), expanded in place
__forceinline float Pow10(float x)
{
    float scale;
    float result;
    float whole;
    float log2of10 = 3.32192802f;                  // 0x40549A78
    float one = 1.0f;
    __asm {
        fld   log2of10
        fmul  x
        frndint
        fst   whole
        wait
        fld1
        fscale
        fxch  st(1)
        fstp  scale
        fld   log2of10
        fmul  x
        fsub  whole
        f2xm1
        fadd  one
        fmulp st(1), st
        fstp  result
        wait
    }
    return result;
}
// libpk's inline-assembly truncation helper: round-to-nearest of (x - 0.5) by a bare fistp, corrected to truncation for negatives
__forceinline long FloatToLongTrunc(float value)
{
    long result;
    value = value - 0.5f;
    __asm fld value
    __asm fistp result
    if (result < 0)
        ++result;
    return result;
}
static const float kRadiansPerHz = 0.00014247586659621447f;   // 0x39156593 @ 0x012BED78: 2*pi / 44100 (a pool constant, the fmul operand)
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
    float r = Pow10((float)(levelMb + levelMb) * 0.0005f);
    float c = (float)cos(referenceFrequency * kRadiansPerHz);
    float a = ((1.0f - r * c) - (float)sqrt(r * ((1.0f - c) * 2.0f - (1.0f - c * c) * r))) * 32768.0f / (1.0f - r);
    long feedback = FloatToLongTrunc(a);
    m_feedback = feedback;
    m_input = 0x7fff - feedback;
    if (m_input == 0)
        m_history = 0;
}