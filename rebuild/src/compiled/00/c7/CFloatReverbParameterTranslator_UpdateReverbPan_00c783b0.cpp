// CFloatReverbParameterTranslator::UpdateReverbPan(CHLRvbParams* pSrcParams) -- turn the high-level reverb pan vector into the short-reverb (four) and long-reverb (two) output gain sets through PanToGains
// Leaf: ?UpdateReverbPan@CFloatReverbParameterTranslator@@AAEXPAVCHLRvbParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C783B0, thiscall, `ret 4`.
// Name (NF99P2, q11507; was the descriptive CChannelMixer_00c783b0::Mix over a local model): GameCODA 2.10 CodeView private
//   ?UpdateReverbPan@CFloatReverbParameterTranslator@@AAEXPAVCHLRvbParams@@@Z. Two sources: (1) this body is byte-identical (relocations
//   masked) to the CODAApi.lib FloatReverbParameterTranslator.obj member of that name, whose two REL32s name PanToGains = retail
//   0x00C77D90; (2) the byte-identical SDK Translate 0x00C783F0 calls it at REL32 +0x162 (retail 0x00C78551) for update flag 0x4000.
// Members: m_fShortRvbPanGain +0x10 (4 gains), m_fLongRvbPanGain +0x20 (2 gains), m_pReverbConstants +0x28 (m_uiNumOutputs +4),
//   CHLRvbParams::m_ReverbPan +0x58 (the CodeView names on the shared headers).
// Shape: the pan block address in edi across both calls, `add esi,20h` for the second record, `ret 4`.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_PANTOGAINS
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"

void CFloatReverbParameterTranslator::UpdateReverbPan(CHLRvbParams* pSrcParams)
{
    PanToGains(&pSrcParams->m_ReverbPan, m_fShortRvbPanGain, 4, m_pReverbConstants->m_uiNumOutputs);
    PanToGains(&pSrcParams->m_ReverbPan, m_fLongRvbPanGain, 2, m_pReverbConstants->m_uiNumOutputs);
}
