// CSalIIRFilter::SetLowPass(int iLevelHF, float fFrequencyRef) -- GameCODA / libpk audio: set a one-pole low-pass filter's Q15 feedback and input weights from an attenuation in millibels (10^(mB/1000) through the x87 2^x block) and a reference frequency; zero resets it
// Leaf: ?SetLowPass@CSalIIRFilter@@QAEXHM@Z
// Retail-asm: 0x00C6C039..0x00C6C069 `fld [esp+8] ; fmul [esp+4] ; frndint ; fst [esp+0Ch] ; wait ; fld1 ; fscale ; fxch st(1) ; fstp [esp+10h] ; fld [esp+8] ; fmul [esp+4] ; fsub [esp+0Ch] ; f2xm1 ; fadd [esp] ; fmulp st(1),st ; fstp [esp+18h] ; wait` -- libpk's inline-assembly 2^x block over named float locals (17 instructions, no branch: rasm1 family (e), the Pow10 helper 0x00C53B10 expanded in place with its result homed in the dead `levelMb` slot); frndint, fscale, f2xm1 and the `wait`s have no C++ spelling in VC7.1 (`pow` becomes a `_CIpow` jump: rasm1 probe). Then 0x00C6C0C4 `fld dword ptr [esp+18h]` ; 0x00C6C0C8 `fistp dword ptr [esp+1Ch]` -- the inline-assembly truncation helper (rasm1 family (a)): the coefficient less 0.5 stored to the same dead slot and reloaded for a bare dword fistp into the dead `referenceFrequency` slot, with the C++ `< 0 ? +1` fix-up. The zero test, the exponent, the cosine/square-root filter arithmetic and the three stores are C++.
// Helper (2026-11-05 asm migration): the 17-instruction x87 2^x block is the shared libpk/Pow10.h (LIBPK_POW10_BLOCK; byte-matching
//   path + FABLE_PORTABLE twin) and libpk/FtoL.h (the truncation helper's bare fistp); the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family POW10.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6C000, 240 bytes, __fastcall (this = the filter in ecx), `ret 8`. Constants:
//   0.0005f @ 0x01264AF0, 2*pi/44100 @ 0x012BED78, 1.0f @ 0x0122DED8, 32768.0f @ 0x0129EF98, 0.5f @ 0x0122F59C;
//   log2(10) = 0x40549A78 as an immediate. The catalog name "Audio_ComputeDecibelsToLinear" is descriptive.
// Name (NF94 q11408): GameCODA 2.10 CODAApi.lib SalSw3DVoice.obj CalcParams release relocations +0x99 / +0x146 (and the Dbg object)
//   name ?SetLowPass@CSalIIRFilter@@QAEXHM@Z, a CodeView public; the shared engine/CSalIIRFilter.h declares it with the
//   CodeView parameter names and the private int members m_iA / m_iB / m_iZ (+0/+4/+8: feedback weight, input weight,
//   history). The landed caller 0x00C54C10 binds this spelling. Was a local COnePoleLowPass model.
#include "fable_compiler.h"
#include "libpk/Pow10.h"
#include "libpk/FtoL.h"
#include <math.h>
#pragma intrinsic(cos, sqrt)
// libpk's inline-assembly 10^x helper (0x00C53B10 is its standalone copy), expanded in place
FABLE_FORCEINLINE float Pow10(float x)
{
    float scale;
    float result;
    float whole;
    float log2of10 = 3.32192802f;                  // 0x40549A78
    float one = 1.0f;
    LIBPK_POW10_BLOCK(log2of10, x, whole, scale, one, result);
    return result;
}
// libpk's inline-assembly truncation helper: round-to-nearest of (x - 0.5) by a bare fistp, corrected to truncation for negatives
FABLE_FORCEINLINE long FloatToLongTrunc(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
static const float kRadiansPerHz = 0.00014247586659621447f;   // 0x39156593 @ 0x012BED78: 2*pi / 44100 (a pool constant, the fmul operand)
#include "engine/CSalIIRFilter.h"
void CSalIIRFilter::SetLowPass(int iLevelHF, float fFrequencyRef)
{
    if (iLevelHF == 0) {
        m_iA = iLevelHF;
        m_iZ = iLevelHF;
        return;
    }
    float r = Pow10((float)(iLevelHF + iLevelHF) * 0.0005f);
    float c = (float)cos(fFrequencyRef * kRadiansPerHz);
    float a = ((1.0f - r * c) - (float)sqrt(r * ((1.0f - c) * 2.0f - (1.0f - c * c) * r))) * 32768.0f / (1.0f - r);
    long feedback = FloatToLongTrunc(a);
    m_iA = feedback;
    m_iB = 0x7fff - feedback;
    if (m_iB == 0)
        m_iZ = 0;
}