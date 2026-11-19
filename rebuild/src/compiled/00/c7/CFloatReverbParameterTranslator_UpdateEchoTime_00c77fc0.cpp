// CFloatReverbParameterTranslator::UpdateEchoTime(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams) -- set the echo feedback delay in samples: round(sample rate * echo time - 0.5) biased up when negative, rounded up to a multiple of 4
// Grade: RELOCATION_MATCH -- retail_asm through the shared helper libpk/FtoL.h (2026-11-05 asm migration, docs/pipeline/ASM_RULES.md); no inline asm in this row
// Leaf: ?UpdateEchoTime@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
// Retail-asm: 0x00C77FE5 `fld dword ptr [esp+0Ch]` + 0x00C77FE9 `fistp dword ptr [esp]` -- libpk's inline-assembly float-to-long helper inside its round-half-down (as 0x00C3FF70: the `value - 0.5f` stored to the dead params slot and reloaded, a bare dword fistp into the `push ecx` frame word). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's inline asm.
// Helper (2026-11-05 asm migration): the bare `fld` / `fistp dword` block is libpk's FtoL in the shared libpk/FtoL.h (byte-matching
//   path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic attempts: asm/w3 ATTEMPTS.tsv family FISTP.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77FC0, length 72 bytes, `ret 8`. Constants 2^32 (0x0122DCB4, unsigned rate fix-up), 0.5f (0x0122F59C).
// Dispatcher bit 0x20000.
// Name (NF99P2, q11501; was the descriptive CReverbProcessor::UpdateChorusLength over local models): GameCODA 2.10 CodeView private
//   ?UpdateEchoTime@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z. Two sources: (1) this body is byte-identical (72 bytes, relocations masked) to the CODAApi.lib FloatReverbParameterTranslator.obj member of that name; (2) the byte-identical SDK CFloatReverbParameterTranslator::Translate 0x00C783F0 calls it at
//   REL32 +0x9f (retail 0x00C7848E). Its only landed binder (0x00C783F0) binds this spelling in this hand-back. Members: the CodeView
//   layouts on engine/CFloatReverbParameterTranslator.h, CFloatReverbParameters.h (+ its params headers), CFloatReverbConstants.h and
//   CHLRvbParams.h (the old local SReverbState / SReverbParams / SMixFormat offsets map one to one).
#include "fable_compiler.h"
#include "libpk/FtoL.h"
FABLE_FORCEINLINE long RoundHalfDownToLong(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"

void CFloatReverbParameterTranslator::UpdateEchoTime(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams)
{
    pReverbParams->m_Echo.m_uiFeedbackDelay =
        (RoundHalfDownToLong((float)m_pReverbConstants->m_uiSamplesPerSecond * pSrcParams->m_fEchoTime) + 3) & ~3;
}
