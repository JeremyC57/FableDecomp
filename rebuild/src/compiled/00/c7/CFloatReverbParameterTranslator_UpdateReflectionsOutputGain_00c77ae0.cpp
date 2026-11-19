// CFloatReverbParameterTranslator::UpdateReflectionsOutputGain(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams) -- set the four reflections' output gains: twice the reflections gain times each reflection's pan gain and a fixed per-reflection trim
// Leaf: ?UpdateReflectionsOutputGain@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77AE0, length 72 bytes (+ int3 padding), `ret 8`. Trim constants 1.05f (0x012777EC) and 1.14f / 0.84f / 0.93f (0x012BF1A4 /
// 0x012BF1A0 / 0x012BF19C). Dispatcher bit 0x1800.
// Name (NF99P2, q11505; was the descriptive CReverbProcessor::UpdateLineGains over local models): GameCODA 2.10 CodeView private
//   ?UpdateReflectionsOutputGain@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z. Two sources: (1) the CODAApi.lib FloatReverbParameterTranslator.obj member of that name has the same shape (m_fReflectionsGain +0x24 times m_fRefPanGain[i] times a per-reflection trim into m_Ref[i].m_fOutputGain +0x48 / +0x5c / +0x70 / +0x84, `ret 8`) but is not byte-identical: the SDK 2.10 build scales by 0.25f (76 bytes) where retail doubles (`fadd st,st`, 72 bytes), another SDK build as LIBRARY_SIGNATURES notes; (2) the byte-identical SDK CFloatReverbParameterTranslator::Translate 0x00C783F0 calls it at
//   REL32 +0x153 (retail 0x00C78542). Its only landed binder (0x00C783F0) binds this spelling in this hand-back. Members: the CodeView
//   layouts on engine/CFloatReverbParameterTranslator.h, CFloatReverbParameters.h (+ its params headers), CFloatReverbConstants.h and
//   CHLRvbParams.h (the old local SReverbState / SReverbParams / SMixFormat offsets map one to one).
// Shape: the doubled gain (`fadd st,st`) stays on the x87 stack (`fld st(0)` per reflection); each gain is
// `(gain * pan) * trim`, stored reflection by reflection. `pSrcParams` is read before `pReverbParams`.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"

void CFloatReverbParameterTranslator::UpdateReflectionsOutputGain(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams)
{
    float gain = pSrcParams->m_fReflectionsGain * 2.0f;
    pReverbParams->m_Ref[0].m_fOutputGain = gain * m_fRefPanGain[0] * 1.05f;
    pReverbParams->m_Ref[1].m_fOutputGain = gain * m_fRefPanGain[1] * 1.14f;
    pReverbParams->m_Ref[2].m_fOutputGain = gain * m_fRefPanGain[2] * 0.84f;
    pReverbParams->m_Ref[3].m_fOutputGain = gain * m_fRefPanGain[3] * 0.93f;
}
