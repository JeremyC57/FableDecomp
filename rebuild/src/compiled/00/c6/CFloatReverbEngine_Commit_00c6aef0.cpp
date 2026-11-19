// CFloatReverbEngine::Commit(CHLRvbParams* pParams, bool bMode) -- commit a high-level reverb parameter block to the float reverb engine: translate its dirty fields into the engine's CFloatReverbParameters (m_Translator, 0x00C783F0), commit those to the DSP engine through its vtable slot 1 (CReverbDspEngine::Commit), then derive the decay length m_iDecayTimeSamples = m_RvbConsts.m_uiSamplesPerSecond * trunc(pParams->GetDecayTime())
// Grade: RELOCATION_MATCH -- retail_asm through the shared helper libpk/FtoL.h (2026-11-05 asm migration, docs/pipeline/ASM_RULES.md); no inline asm in this row
// Leaf: ?Commit@CFloatReverbEngine@@QAEXPAVCHLRvbParams@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6AEF0, 92 bytes (+ int3 padding), `ret 8`.
// Name (NF99P2, q11498; was the descriptive CVoiceEffect::Configure over a local model): GameCODA 2.10 CodeView public
//   ?Commit@CFloatReverbEngine@@QAEXPAVCHLRvbParams@@_N@Z. Two sources: (1) this body is byte-identical (92 bytes, relocations masked)
//   to the CODAApi.lib FloatReverbEngine.obj member of that name (relocations: Translate REL32 +0x11, 0.5f); (2) its REL32 +0x11
//   lands on 0x00C783F0, itself byte-identical to the SDK CFloatReverbParameterTranslator::Translate, and the body reads
//   CHLRvbParams::m_fDecayTime (+0x10) and the CReverbConstants sample rate at engine +0x23c (m_RvbConsts +0x234 + 8). The landed
//   sibling CFloatReverbEngine::Create 0x00C6B050 writes the same Translate / slot-1 Commit / decay sequence on the same header.
// Callers: CDecodedStream::Initialise 0x00C544FE / CDecodedStream 0x00C546BE, on the engine at stream +0x2f8 with the stream's
//   +0x290 CHLRvbParams block.
// Callees: CFloatReverbParameterTranslator::Translate 0x00C783F0 (m_Translator +0x260), the DSP engine's vtable slot 1
//   (CReverbDspEngine::Commit(void*, bool), m_DspEngine +0x28c, called through a base pointer as in 0x00C6B050: a member called by
//   name is devirtualised).
// Retail-asm: 0x00C6AF1A `fld [edi+10h] ; fsub [0.5f] ; fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp+10h] ; test ; jge ; inc` -- libpk's inline-assembly truncation helper (value homed in the dead `pParams` slot, result in the dead `bMode` slot). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's inline asm.
// Helper (2026-11-05 asm migration): the bare `fld` / `fistp dword` block is libpk's FtoL in the shared libpk/FtoL.h (byte-matching
//   path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic attempts: asm/w3 ATTEMPTS.tsv family FISTP.
#include "fable_compiler.h"
#include "libpk/FtoL.h"
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
// (retail: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`)
FABLE_FORCEINLINE long FloatToLongTruncate(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBENGINE_COMMIT
#include "engine/CFloatReverbEngine.h"
#include "engine/CHLRvbParams.h"

void CFloatReverbEngine::Commit(CHLRvbParams* pParams, bool bMode)
{
    m_Translator.Translate(&m_RvbParams, pParams);
    CReverbDspEngine* pEngine = &m_DspEngine;
    pEngine->Commit(&m_RvbParams, bMode);
    m_iDecayTimeSamples = m_RvbConsts.m_uiSamplesPerSecond * FloatToLongTruncate(pParams->GetDecayTime());
}
