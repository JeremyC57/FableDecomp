// CFloatReverbParameterTranslator::UpdateDecayTimes(CFloatReverbParameters*,CHLRvbParams*) -- turn the high-level decay time and LF/HF decay ratios into the feedback gains and shelf filters of the short reverbs, the long reverbs, the long-reverb input filters and the echo
// Leaf: ?UpdateDecayTimes@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
// Retail: Fable.exe TLC @0x00C77800, 725 bytes, ret 8.
// Source: GameCODA 2.10 CODAApiDbg.lib FloatReverbParameterTranslator.obj CodeView (locals, scopes, statement and operand order).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"
#include <math.h>

void CFloatReverbParameterTranslator::UpdateDecayTimes(CFloatReverbParameters *pReverbParams, CHLRvbParams *pSrcParams)
{
    float fDecaySamples;
    float fDecayFactorHF;
    unsigned int i;
    float fDecayFactorLF;
    float fGainRatioHF;
    unsigned int uiFilterSpacing;
    float fGainRatioLF;
    float fGain;

    // Samples for a 60 dB decay: exp(-n / fDecaySamples) is the gain of an n-sample delay.
    fDecaySamples = (float)m_pReverbConstants->m_uiSamplesPerSecond * pSrcParams->m_fDecayTime / 6.907755f;
    fDecayFactorLF = (pSrcParams->m_fDecayTimeLFRatio - 1.0f) / (fDecaySamples * pSrcParams->m_fDecayTimeLFRatio);
    fDecayFactorHF = (pSrcParams->m_fDecayTimeHFRatio - 1.0f) / (fDecaySamples * pSrcParams->m_fDecayTimeHFRatio);

    // Short reverb feedback filters
    for (i = 0; i < 4; i++)
    {
        unsigned int uiBufferSamples = m_pReverbConstants->m_ShortRvbConstants[i].uiBufferLength;
        fGain = expf(-((float)uiBufferSamples / fDecaySamples));
        pReverbParams->m_ShortReverb.m_aShortRvb[i].m_fFilter.SetGain(fGain);
        fGainRatioLF = expf((float)uiBufferSamples * fDecayFactorLF);
        fGainRatioHF = expf((float)uiBufferSamples * fDecayFactorHF);
        pReverbParams->m_ShortReverb.m_aShortRvb[i].m_fFilter.Set_ShelfLfHf(fGainRatioLF, pSrcParams->m_fRefFrequencyLF, fGainRatioHF, pSrcParams->m_fRefFrequencyHF, m_pReverbConstants->m_uiSamplesPerSecond);
    }

    // Long reverb feedback filters
    for (i = 0; i < 2; i++)
    {
        unsigned int uiBufferSamples = pReverbParams->m_LongReverb.m_aLongRvb[i].m_uiFeedbackDelay;
        fGain = expf(-((float)uiBufferSamples / fDecaySamples));
        pReverbParams->m_LongReverb.m_aLongRvb[i].m_fFeedbackFilter.SetGain(fGain);
        fGainRatioLF = expf((float)uiBufferSamples * fDecayFactorLF);
        fGainRatioHF = expf((float)uiBufferSamples * fDecayFactorHF);
        pReverbParams->m_LongReverb.m_aLongRvb[i].m_fFeedbackFilter.Set_ShelfLfHf(fGainRatioLF, pSrcParams->m_fRefFrequencyLF, fGainRatioHF, pSrcParams->m_fRefFrequencyHF, m_pReverbConstants->m_uiSamplesPerSecond);
    }

    // Long reverb input filters
    uiFilterSpacing = m_pReverbConstants->m_uiMaxLongReverbInputSamples >> 2;
    for (i = 0; i < 3; i++)
    {
        unsigned int uiBufferSamples = 0;
        for (unsigned int j = 0; j < 2; j++)
            uiBufferSamples += pReverbParams->m_LongReverb.m_aLongRvb[j].m_auiInputDelay[i + 1];
        uiBufferSamples = uiBufferSamples / 2 - (i + 1) * uiFilterSpacing - m_pReverbConstants->m_uiMaxShortReverbInputSamples;
        fGain = expf(-((float)uiBufferSamples / fDecaySamples));
        pReverbParams->m_LongReverb.m_aInputFilter[i].SetGain(fGain);
        fGainRatioLF = expf((float)uiBufferSamples * fDecayFactorLF);
        fGainRatioHF = expf((float)uiBufferSamples * fDecayFactorHF);
        pReverbParams->m_LongReverb.m_aInputFilter[i].Set_ShelfLfHf(fGainRatioLF, pSrcParams->m_fRefFrequencyLF, fGainRatioHF, pSrcParams->m_fRefFrequencyHF, m_pReverbConstants->m_uiSamplesPerSecond);
    }

    // Echo feedback gain
    fGain = expf(-((float)m_pReverbConstants->m_uiSamplesPerSecond * pSrcParams->m_fEchoTime / fDecaySamples));
    pReverbParams->m_Echo.m_fFeedbackGain = fGain;
}