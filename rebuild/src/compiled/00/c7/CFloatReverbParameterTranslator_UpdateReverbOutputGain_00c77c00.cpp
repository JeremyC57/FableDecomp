// CFloatReverbParameterTranslator::UpdateReverbOutputGain(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams) -- set the four short-reverb and two long-reverb output gains from 0.3 * the reverb gain and the pan gains, then the echo depth (2 - diffusion) * echo depth clamped to 1
// Leaf: ?UpdateReverbOutputGain@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77C00, length 125 bytes (+ int3 padding), `ret 8`. Constants 0.3f (0x01238170), 2.0f (0x0122DCDC), 1.0f (0x0122DED8).
// Dispatcher bits 0xC000.
// Name (NF99P2, q11506; was the descriptive CReverbProcessor::UpdateTapLevels over local models): GameCODA 2.10 CodeView private
//   ?UpdateReverbOutputGain@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z. Two sources: (1) this body is byte-identical (125 bytes, relocations masked) to the CODAApi.lib FloatReverbParameterTranslator.obj member of that name; (2) the byte-identical SDK CFloatReverbParameterTranslator::Translate 0x00C783F0 calls it at
//   REL32 +0x172 (retail 0x00C78561). Its only landed binder (0x00C783F0) binds this spelling in this hand-back. Members: the CodeView
//   layouts on engine/CFloatReverbParameterTranslator.h, CFloatReverbParameters.h (+ its params headers), CFloatReverbConstants.h and
//   CHLRvbParams.h (the old local SReverbState / SReverbParams / SMixFormat offsets map one to one).
// Shape: the scaled level stays on the x87 stack for the six products; the mix is a named local stored with
// `fst`, then compared (`fcomp 1.0 ; test ah,41h ; jne`) and overwritten with 1.0f when larger.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"

void CFloatReverbParameterTranslator::UpdateReverbOutputGain(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams)
{
    float level = pSrcParams->m_fReverbGain * 0.3f;
    pReverbParams->m_ShortReverb.m_aShortRvb[0].m_fOutputGain = level * m_fShortRvbPanGain[0];
    pReverbParams->m_ShortReverb.m_aShortRvb[1].m_fOutputGain = level * m_fShortRvbPanGain[1];
    pReverbParams->m_ShortReverb.m_aShortRvb[2].m_fOutputGain = level * m_fShortRvbPanGain[2];
    pReverbParams->m_ShortReverb.m_aShortRvb[3].m_fOutputGain = level * m_fShortRvbPanGain[3];
    pReverbParams->m_LongReverb.m_aLongRvb[0].m_fOutputGain = level * m_fLongRvbPanGain[0];
    pReverbParams->m_LongReverb.m_aLongRvb[1].m_fOutputGain = level * m_fLongRvbPanGain[1];
    float mix = (2.0f - pSrcParams->m_fDiffusion) * pSrcParams->m_fEchoDepth;
    pReverbParams->m_Echo.m_fDepth = mix;
    if (mix > 1.0f)
        pReverbParams->m_Echo.m_fDepth = 1.0f;
}
