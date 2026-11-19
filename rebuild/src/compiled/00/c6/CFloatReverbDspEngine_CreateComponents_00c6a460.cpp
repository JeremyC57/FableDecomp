// CFloatReverbDspEngine::CreateComponents(CFloatReverbConstants* pConstants, void* pvMemory, int* piBytes) -- two-pass memory layout of the float reverb engine: restart the fader tokens, size (and with memory, carve) the engine's own reverb buffers, clear both input filters, then lay out the four reflections (a fifth of a second plus 128 samples, multiple of 4), the short reverb, the long reverb, the echo, the pitch shifter and the diffuser from one cursor, and return the total bytes
// Leaf: ?CreateComponents@CFloatReverbDspEngine@@AAEXPAVCFloatReverbConstants@@PAXPAH@Z
// Flags: none
// Retail-asm: 0x00C6A4CE `fld dword ptr [esp+1Ch]` + 0x00C6A4D2 `fistp dword ptr [esp+14h]` -- libpk's inline-assembly float-to-long helper inside its round-half-down (as 0x00C3FF70 and the landed CReverbProcessor::UpdateChorusLength 0x00C77FC0: the `rate * 0.2f - 0.5f` stored to a dead frame slot and reloaded, a bare dword fistp, then the C++ `+1 if negative`). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` into an 8-byte slot: no C/C++ form gives the bare dword fistp, so it is the original's inline asm.
// Helper (2026-11-05 asm migration): the bare `fld` / `fistp dword` block is libpk's FtoL in the shared libpk/FtoL.h (byte-matching
//   path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic attempts: asm/w3 ATTEMPTS.tsv family FISTP.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6A460, 364 bytes, thiscall `ret 0Ch`, 8-byte frame (the running byte total and
//   the reflection length / loop counter slots). Constants 2^32 (0x0122DCB4, unsigned rate fix-up), 0.2f (0x012305A0),
//   0.5f (0x0122F59C).
// Name (NF105, q11546; was the descriptive CReverbProcessor::Allocate over local models): GameCODA 2.10 CodeView private
//   ?CreateComponents@CFloatReverbDspEngine@@AAEXPAVCFloatReverbConstants@@PAXPAH@Z. Two sources: (1) this body is byte-identical
//   (364 bytes, relocations masked) to that CODAApi.lib FloatReverbDspEngine.obj member, whose relocations name the retail callees
//   (+0x2c CreateReverbBuffers 0x00C69A70, +0x34 / +0x3f CFloatFilter::Clear 0x00C744E0, +0xb1 CFloatReflection::Create 0x00C75F80,
//   +0xe2 CFloatShortReverb::Create 0x00C75C30, +0xf9 CFloatLongReverb::Create 0x00C755B0, +0x115 CFloatEcho::Create 0x00C76AB0,
//   +0x136 CFloatPitch::Create 0x00C76DD0, +0x154 CFloatDiffuser::Create 0x00C75050); (2) the byte-identical SDK
//   CFloatReverbDspEngine::Create 0x00C6A8B0 calls it twice (REL32 +0xdb / +0x116), the retail call sites. Layout: the CodeView
//   CReverbDspEngine (m_iFaderToken +0x18) and CFloatReverbDspEngine (m_iInputFilterFaderToken +0x290); the constants are
//   CFloatReverbConstants (m_uiNumOutputs +4, m_uiSamplesPerSecond +8, m_ShortRvbConstants +0x1c).
// Descriptive: RoundHalfDownToLong is the landed descriptive inline helper (no retail symbol), unchanged.
#include "fable_compiler.h"
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATREVERBDSPENGINE_CREATECOMPONENTS
#define FABLE_RETAIL_CFLOATREFLECTION_CREATE
#define FABLE_RETAIL_CFLOATSHORTREVERB_CREATE
#define FABLE_RETAIL_CFLOATLONGREVERB_CREATE
#define FABLE_RETAIL_CFLOATECHO_CREATE
#define FABLE_RETAIL_CFLOATPITCH_CREATE
#define FABLE_RETAIL_CFLOATDIFFUSER_CREATE
#include "engine/CFloatReverbDspEngine.h"
// libpk's inline-assembly float-to-long helper (round to nearest, the FPU default)
#include "libpk/FtoL.h"
// libpk's round-half-down helper (out-of-line copy at 0x00C3FF70): round(value - 0.5), negatives step back up by one
FABLE_FORCEINLINE long RoundHalfDownToLong(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

void CFloatReverbDspEngine::CreateComponents(CFloatReverbConstants* pConstants, void* pvMemory, int* piBytes)
{
    m_iFaderToken = 0;
    int iBytes = 0;
    CreateReverbBuffers(pConstants, pvMemory, &iBytes);
    m_inputFilterLowPass.Clear();
    m_inputFilterHighPass.Clear();
    m_iInputFilterFaderToken = m_iFaderToken++;
    unsigned int uiLength = (RoundHalfDownToLong((float)pConstants->m_uiSamplesPerSecond * 0.2f) + 128 + 3) & ~3;
    for (int i = 0; i < 4; ++i) {
        m_reflection[i].Create(uiLength, pvMemory, &iBytes, m_iFaderToken);
        m_iFaderToken++;
    }
    m_ShortReverb.Create(pConstants->m_uiNumOutputs, pConstants->m_ShortRvbConstants, pvMemory, &iBytes, m_iFaderToken);
    m_LongReverb.Create(pConstants, pvMemory, &iBytes, m_iFaderToken);
    m_echo.Create(pConstants->m_uiSamplesPerSecond, pvMemory, &iBytes, m_iFaderToken);
    m_Pitch.Create(pConstants->m_uiSamplesPerSecond, pvMemory, &iBytes, ++m_iFaderToken);
    m_iFaderToken++;
    m_diffuser.Create(pConstants->m_uiSamplesPerSecond, pvMemory, &iBytes);
    *piBytes = iBytes;
}
