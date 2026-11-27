// 00c76120
// Leaf: ?Process@CFloatReflection@@QAEXPAUCFloatReflectionParams@@PBU2@PAVCFloatCyclicBuffer@@PAM3II@Z
// Flags: none
#define FABLE_RETAIL_CFLOATREFLECTION_PROCESS_FADING
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATREFLECTION_PROCESS
#include "engine/CFloatReflectionParams.h"
#include "engine/CFloatReflection.h"

void CFloatReflection::Process(CFloatReflectionParams* pParams0, const CFloatReflectionParams* pParams1,
                              CFloatCyclicBuffer* pInputBuffer, float* pfInternalBuffer, float* pfOutputBuffer,
                              unsigned uiSamples, unsigned uiSamplesToFade)
{
    float* pfBase = m_Buffer.m_pBase;
    unsigned iWrFeedbackCursor = m_Buffer.GetWriteCursor();
    float* pfInputBase = pInputBuffer->m_pBase;
    unsigned iRdFeedbackCursor0 = iWrFeedbackCursor >= pParams0->m_uiFeedbackDelay
        ? iWrFeedbackCursor - pParams0->m_uiFeedbackDelay
        : iWrFeedbackCursor - pParams0->m_uiFeedbackDelay + m_Buffer.GetLength();
    unsigned iRdFeedbackCursor1 = iWrFeedbackCursor >= pParams1->m_uiFeedbackDelay
        ? iWrFeedbackCursor - pParams1->m_uiFeedbackDelay
        : iWrFeedbackCursor - pParams1->m_uiFeedbackDelay + m_Buffer.GetLength();
    unsigned iWrInputCursor = pInputBuffer->GetWriteCursor() >= uiSamples
        ? pInputBuffer->GetWriteCursor() - uiSamples
        : pInputBuffer->GetWriteCursor() - uiSamples + pInputBuffer->GetLength();
    float fFadeOut = (float)uiSamplesToFade / 512.0f;
    float fFadeIn = 1.0f - fFadeOut;
    unsigned uiSamplesToProcess = uiSamples;
    if (uiSamplesToFade < uiSamples)
        uiSamplesToProcess = uiSamplesToFade;
    for (unsigned i = 0; i < uiSamplesToProcess; ++i) {
        unsigned iRdInputCursor0 = iWrInputCursor >= pParams0->m_auiInputDelay[0]
            ? iWrInputCursor - pParams0->m_auiInputDelay[0]
            : iWrInputCursor - pParams0->m_auiInputDelay[0] + pInputBuffer->GetLength();
        unsigned iRdInputCursor1 = iWrInputCursor >= pParams1->m_auiInputDelay[0]
            ? iWrInputCursor - pParams1->m_auiInputDelay[0]
            : iWrInputCursor - pParams1->m_auiInputDelay[0] + pInputBuffer->GetLength();
        float fSample0 = pfInputBase[iRdInputCursor0];
        float fSample1 = pfInputBase[iRdInputCursor1];
        *pfInternalBuffer += fSample0*fFadeOut + fSample1*fFadeIn;
        ++pfInternalBuffer;
        for (unsigned iInput = 1; iInput > 0; --iInput) {
            iRdInputCursor0 = iWrInputCursor >= pParams0->m_auiInputDelay[iInput]
                ? iWrInputCursor - pParams0->m_auiInputDelay[iInput]
                : iWrInputCursor - pParams0->m_auiInputDelay[iInput] + pInputBuffer->GetLength();
            iRdInputCursor1 = iWrInputCursor >= pParams1->m_auiInputDelay[iInput]
                ? iWrInputCursor - pParams1->m_auiInputDelay[iInput]
                : iWrInputCursor - pParams1->m_auiInputDelay[iInput] + pInputBuffer->GetLength();
            fSample0 += pfInputBase[iRdInputCursor0];
            fSample1 += pfInputBase[iRdInputCursor1];
        }
        fSample0 += pfBase[iRdFeedbackCursor0] * pParams0->m_fFeedbackGain;
        fSample1 += pfBase[iRdFeedbackCursor1] * pParams1->m_fFeedbackGain;
        // Bracket: the grouped feedback blend keeps the SDK feedback-store and cursor-update instruction order.
        pfBase[iWrFeedbackCursor] = (fSample0*fFadeOut + fSample1*fFadeIn);
        *pfOutputBuffer += fFadeOut*(fSample0*pParams0->m_fOutputGain)
                         + fFadeIn*(fSample1*pParams1->m_fOutputGain);
        ++pfOutputBuffer;
        iRdFeedbackCursor0 = iRdFeedbackCursor0+1 < m_Buffer.GetLength()
            ? iRdFeedbackCursor0+1 : iRdFeedbackCursor0+1-m_Buffer.GetLength();
        iRdFeedbackCursor1 = iRdFeedbackCursor1+1 < m_Buffer.GetLength()
            ? iRdFeedbackCursor1+1 : iRdFeedbackCursor1+1-m_Buffer.GetLength();
        iWrFeedbackCursor = iWrFeedbackCursor+1 < m_Buffer.GetLength()
            ? iWrFeedbackCursor+1 : iWrFeedbackCursor+1-m_Buffer.GetLength();
        iWrInputCursor = iWrInputCursor+1 < pInputBuffer->GetLength()
            ? iWrInputCursor+1 : iWrInputCursor+1-pInputBuffer->GetLength();
        if (fFadeOut > 1.0f/512.0f) {
            fFadeIn += 1.0f/512.0f;
            fFadeOut -= 1.0f/512.0f;
        }
    }
    m_Buffer.SetWriteCursor(iWrFeedbackCursor);
    if (uiSamples >= uiSamplesToFade)
        *pParams0 = *pParams1;
}