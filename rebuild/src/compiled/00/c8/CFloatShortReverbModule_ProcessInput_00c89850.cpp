// 00c89850 CFloatShortReverbModule::ProcessInput
// Leaf: ?ProcessInput@CFloatShortReverbModule@@QAEXPAUCFloatShortReverbModuleParams@@PBU2@PBVCFloatCyclicBuffer@@PAM3II@Z
// Flags: none
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT_FADING
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
#include <string.h>
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatShortReverbModuleParams.h"
#include "engine/CFloatShortReverbModule.h"


void CFloatShortReverbModule::ProcessInput(CFloatShortReverbModuleParams* pParamsFrom, const CFloatShortReverbModuleParams* pParamsTo,
    const CFloatCyclicBuffer* pInputBuffer, float* pfInternalBuffer, float* pfOutputBuffer,
    unsigned int uiSamples, unsigned int uiSamplesToFade)
{
    float* pfBase = m_Buffer.m_pBase;
    unsigned int iRdFeedbackCursor = m_Buffer.GetWriteCursor();
    unsigned int iWrInputCursor = pInputBuffer->GetWriteCursor() >= uiSamples
        ? pInputBuffer->GetWriteCursor() - uiSamples : pInputBuffer->GetWriteCursor() - uiSamples + pInputBuffer->GetLength();
    float fFadeOut = (float)uiSamplesToFade / 512.0f;
    float fFadeIn = 1.0f - fFadeOut;
    float fFadeOutFinal = ((float)uiSamplesToFade - (float)uiSamples) / 512.0f;
    float fFadeInFinal = 1.0f - fFadeOutFinal;
    memset(pfInternalBuffer, 0, uiSamples * sizeof(float));
    unsigned int uiSamplesToFilter = uiSamples;
    if (iRdFeedbackCursor + uiSamples > m_Buffer.GetLength())
        uiSamplesToFilter = m_Buffer.GetLength() - iRdFeedbackCursor;
    m_Filter.Process(&pParamsFrom->m_fFilter, &pParamsTo->m_fFilter,
        pfBase + iRdFeedbackCursor, pfInternalBuffer, true, uiSamplesToFilter, uiSamplesToFade);
    if (uiSamplesToFilter < uiSamples)
        m_Filter.Process(&pParamsFrom->m_fFilter, &pParamsTo->m_fFilter,
            pfBase, pfInternalBuffer + uiSamplesToFilter, true, uiSamples - uiSamplesToFilter, uiSamplesToFade);
    for (unsigned int iInput = 0; iInput < 2; ++iInput) {
        float fScale = pParamsFrom->m_afInputGain[iInput];
        unsigned int iRdInputCursor = iWrInputCursor >= pParamsFrom->m_auiInputDelay[iInput]
            ? iWrInputCursor - pParamsFrom->m_auiInputDelay[iInput] : iWrInputCursor - pParamsFrom->m_auiInputDelay[iInput] + pInputBuffer->GetLength();
        m_pDSP->ScaleAddSamples(pfInternalBuffer,pInputBuffer,iRdInputCursor,
            fScale * fFadeOut, (fFadeOutFinal - fFadeOut) * fScale / (float)uiSamples,uiSamples);
        fScale = pParamsTo->m_afInputGain[iInput];
        iRdInputCursor = iWrInputCursor >= pParamsTo->m_auiInputDelay[iInput]
            ? iWrInputCursor - pParamsTo->m_auiInputDelay[iInput] : iWrInputCursor - pParamsTo->m_auiInputDelay[iInput] + pInputBuffer->GetLength();
        m_pDSP->ScaleAddSamples(pfInternalBuffer,pInputBuffer,iRdInputCursor,
            fScale * fFadeIn, (fFadeInFinal - fFadeIn) * fScale / (float)uiSamples,uiSamples);
    }
    float fScaleStart = fFadeOut * pParamsFrom->m_fOutputGain + fFadeIn * pParamsTo->m_fOutputGain;
    float fScaleFinish = fFadeOutFinal * pParamsFrom->m_fOutputGain + fFadeInFinal * pParamsTo->m_fOutputGain;
    m_pDSP->ScaleAddSamples(pfOutputBuffer,pfInternalBuffer,fScaleStart,(fScaleFinish-fScaleStart)/(float)uiSamples,uiSamples);
    if (uiSamples >= uiSamplesToFade) *pParamsFrom = *pParamsTo;
}