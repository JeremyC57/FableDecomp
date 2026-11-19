// CFloatReverbParameterTranslator::UpdateModulationDepth(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams) -- set the pitch shifter's modulation amplitude in samples (modulation depth * modulation time * 0.035014085 * sample rate), limited to the constants' maximum modulation length
// Leaf: ?UpdateModulationDepth@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C78040, length 116 bytes (+ int3 padding), `ret 8`. Constants 0.035014085f (0x012BF1A8), 2^32 (0x0122DCB4: the unsigned
// int-to-float fix-up). Dispatcher bits 0x180000.
// Name (NF99P2, q11502; was the descriptive CReverbProcessor::UpdateDelayLength over local models): GameCODA 2.10 CodeView private
//   ?UpdateModulationDepth@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z. Two sources: (1) this body is byte-identical (116 bytes, relocations masked) to the CODAApi.lib FloatReverbParameterTranslator.obj member of that name; (2) the byte-identical SDK CFloatReverbParameterTranslator::Translate 0x00C783F0 calls it at
//   REL32 +0xfb (retail 0x00C784EA). Its only landed binder (0x00C783F0) binds this spelling in this hand-back. Members: the CodeView
//   layouts on engine/CFloatReverbParameterTranslator.h, CFloatReverbParameters.h (+ its params headers), CFloatReverbConstants.h and
//   CHLRvbParams.h (the old local SReverbState / SReverbParams / SMixFormat offsets map one to one).
// Shape: the parameter product is a named local formed before the rate conversion (one expression with the
// conversion last lets VC7.1 evaluate the `fild` first). Both unsigned constants fields convert through `fild ; jge ; fadd 2^32`; the limit is converted again (and
// m_pReverbConstants re-read) in the taken branch, so the source spells the conversion twice in an if/else -- each arm
// ends in its own `fstp [params+224h] ; ret 8`.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"

void CFloatReverbParameterTranslator::UpdateModulationDepth(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams)
{
    float scaled = pSrcParams->m_fModulationDepth * pSrcParams->m_fModulationTime * 0.035014085f;
    float length = scaled * (float)m_pReverbConstants->m_uiSamplesPerSecond;
    if (length > (float)m_pReverbConstants->m_uiMaxModulationSamples)
        pReverbParams->m_PitchShift.m_fModulationAmplitudeSamples = (float)m_pReverbConstants->m_uiMaxModulationSamples;
    else
        pReverbParams->m_PitchShift.m_fModulationAmplitudeSamples = length;
}
