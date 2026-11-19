// CSalSwBaseVoice::SetPitch(float fPitch, const CODASOURCE& params) -- recompute the software voice's resampling rate: the pitch times the active pitch modulators (multi-function envelope, pitch LFO, multi-function LFO), times source rate / mixer rate, clamped below at 0.01, in 20.12 fixed point (x 4096, rounded through the retail inline-asm `fistp`), capped at 0xF000
// Grade: RELOCATION_MATCH -- retail_asm through the shared helper libpk/FtoL.h (2026-11-05 asm migration, docs/pipeline/ASM_RULES.md); no inline asm in this row
// Retail-asm: 0x00C5A57C `fstp [esp+4]` / 0x00C5A580 `fld dword ptr [esp+4]` + 0x00C5A584 `fistp dword ptr [esp+8]` -- GameCODA's inline-assembly float-to-long helper (its by-value copy homed in the dead `pitch` slot and reloaded, a bare dword fistp into the dead `desc` slot, then the C++ `+1 if negative` truncation fix-up); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's inline asm.
// Helper (2026-11-05 asm migration): the bare `fld` / `fistp dword` block is libpk's FtoL in the shared libpk/FtoL.h (byte-matching
//   path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic attempts: asm/w3 ATTEMPTS.tsv family FISTP.
// Leaf: ?SetPitch@CSalSwBaseVoice@@IAEXMABUCODASOURCE@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5A500, length 171 bytes (+ int3 padding); `ret 8`.
// Name (NF99, q11405; was the descriptive CSoftwareVoice::UpdateStep over a local voice model): GameCODA 2.10 CODAApi.lib
//   SalSwBaseVoice.obj ?SetPitch@CSalSwBaseVoice@@IAEXMABUCODASOURCE@@@Z (171 bytes, the same nine data relocations: 2^32,
//   g_SalCaps twice, 0.01f twice, 4096.0f, 0.5f twice) + the SDK SalSw3DVoice.obj CalcParams REL32 that the landed caller
//   0x00C54C10 already binds under this spelling (engine/CSalSwBaseVoice.h, opt-in FABLE_RETAIL_CSALSW3D_CALCPARAMS).
// Globals: g_SalCaps.SWMixer.nSamplesPerSec 0x0139C9E8 (g_SalCaps 0x0139C988 + 0x60; was the local `g_outputRate`).
// Callers: 0x00C54C37 (CSalSw3DVoice::CalcParams), 0x00C5CA7C. No callees.
// Members: the SDK CodeView names on the shared headers: m_fMultiEnvPitch +0x74, m_fPitchLFOPitch +0x3c, m_fMultiLFOPitch +0x88,
//   m_BufferDesc.nSamplesPerSec +0x60, m_uiSrcRate +0x1e8 (six-channel history: FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23).
// Shape: each modulator multiplies only when the source parameters carry it (`mov edx,[eax+1Ch] ; test ; je ; fmul`);
// both rates are unsigned conversions (`fild ; jge ; fadd 2^32`); `< 0.01f` is `fcom ; test ah,5 ; jp`; the
// unfolded `+ 0.5f - 0.5f` precedes the ROUND; the cap stores first then re-stores 0xF000 (`cmp ; mov ; jbe`).
// the inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#include "fable_compiler.h"
#include "libpk/FtoL.h"
#include "engine/CodaSourceTypes.h"
#include "engine/CSalSwBaseVoice.h"
#include "engine/CodaCaps.h"
// truncation toward zero on top of it: round(x - 0.5) is floor(x), negatives step back up by one
FABLE_FORCEINLINE long FloatToLongTruncate(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

void CSalSwBaseVoice::SetPitch(float fPitch, const CODASOURCE& params)
{
    if (params.pMultiEnv)
        fPitch *= m_fMultiEnvPitch;
    if (params.pPitchLFO)
        fPitch *= m_fPitchLFOPitch;
    if (params.pMultiLFO)
        fPitch *= m_fMultiLFOPitch;
    float ratio = (float)m_BufferDesc.nSamplesPerSec * fPitch / (float)g_SalCaps.SWMixer.nSamplesPerSec;
    if (ratio < 0.01f)
        ratio = 0.01f;
    m_uiSrcRate = FloatToLongTruncate(ratio * 4096.0f + 0.5f);
    if (m_uiSrcRate > 0xf000)
        m_uiSrcRate = 0xf000;
}
