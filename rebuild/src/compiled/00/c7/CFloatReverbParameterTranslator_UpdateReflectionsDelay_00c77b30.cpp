// CFloatReverbParameterTranslator::UpdateReflectionsDelay(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams) -- set the four early reflections' input delays in samples: input 0 to the reflections delay, input 1 to that plus the reverb-delay-scaled default input delay of each reflection, each rounded up to a multiple of 4
// Leaf: ?UpdateReflectionsDelay@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C77B30, 193 bytes, thiscall `ret 8`.
// Grade: RELOCATION_MATCH (193/193), retail_asm through the shared libpk/FtoL.h only (no asm in this row); no flag, no pragma, no device.
// Name (was the descriptive CReverbProcessor::UpdateLineLengths over local models): GameCODA 2.10 CodeView private
//   ?UpdateReflectionsDelay@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z. Two sources: (1) this
//   body is byte-identical (193 bytes, relocations masked) to that CODAApi.lib FloatReverbParameterTranslator.obj release member, whose
//   DIR32 at +0x6f names the table _sc_DefaultReflectionData; (2) the landed byte-identical Translate 0x00C783F0 calls it (REL32 +0x11d,
//   rebuild/oracles/reloc_bindings.tsv binds 0x00C77B30 to this symbol).
// Text: the CODAApiDbg.lib member's own body (debug 0x1800..0x1C10, 1376 bytes): the locals uiReflectionsDelaySamples, iReflection,
//   iInput, uiInputDelaySamples; `/ 0.05f` (debug fdiv 0x3D4CCCCD; the release folds it into the 20.0f multiply, `* 20.0f` gives the
//   same bytes); the inner iInput loop from 1 to 2 indexing fInputDelay[iInput - 1] (debug [table + 0xc + 0x14*iReflection + 4*iInput];
//   without the loop retail's `mov eax,4 ... cmp eax,54h` induction becomes `xor eax,eax ... cmp eax,50h`, DIFFER(190v193)). The debug
//   build's ASSERTs and OutputDebugPrintf traces compile away in release. ds_ftoi is the SDK's ReverbMath.h header inline
//   (?ds_ftoi@@YAHM@Z), a plain inline here (new shared engine/ReverbMath.h); ReflectionData / sc_DefaultReflectionData from the
//   CodeView types (new shared engine/ReflectionData.h).
// Retail-asm: 0x00C77B59 `fld dword ptr [esp+1Ch]` + 0x00C77B5D `fistp dword ptr [esp+10h]`, 0x00C77BC1 + 0x00C77BC5 `fistp dword ptr [esp+18h]`:
//   ds_ftoi's bare dword fld / fistp (the debug ds_ftoi 0x1D36 / 0x1D39 has it too); VC7.1 `(long)x` calls __ftol2 and /QIfist emits
//   `fistp qword`, so it is the original's inline asm, from the shared libpk/FtoL.h helper.
#include "fable_compiler.h"
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"
#include "engine/ReflectionData.h"
#include "engine/ReverbMath.h"

void CFloatReverbParameterTranslator::UpdateReflectionsDelay(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams)
{
    unsigned int uiReflectionsDelaySamples = (ds_ftoi((float)m_pReverbConstants->m_uiSamplesPerSecond * pSrcParams->m_fReflectionsDelay) + 3) & ~3;
    for (unsigned int iReflection = 0; iReflection < 4; iReflection++) {
        pReverbParams->m_Ref[iReflection].m_auiInputDelay[0] = uiReflectionsDelaySamples;
        for (unsigned int iInput = 1; iInput < 2; iInput++) {
            unsigned int uiInputDelaySamples = ds_ftoi((pSrcParams->m_fReverbDelay / 0.05f + 1.0f) * 0.1f
                                                       * (float)m_pReverbConstants->m_uiSamplesPerSecond
                                                       * sc_DefaultReflectionData[iReflection].fInputDelay[iInput - 1])
                                               + uiReflectionsDelaySamples;
            pReverbParams->m_Ref[iReflection].m_auiInputDelay[iInput] = (uiInputDelaySamples + 3) & ~3;
        }
    }
}