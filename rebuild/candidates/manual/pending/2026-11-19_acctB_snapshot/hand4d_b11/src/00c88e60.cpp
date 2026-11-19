// CReverbFnsSSE::ApplyFilterOrder2_quad(float*,const float*,float**,CFloatFilterParams**,unsigned int) -- run four second-order filters side by side over four 128-sample channel blocks with SSE: gather the four filters' coefficients, gains and two-sample histories into one aligned block, run the shared SSE kernel, write the histories back
// Leaf: ?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z
// Retail: Fable.exe TLC @0x00C88E60, 681 bytes, realigned (`and esp,-16` through ebx) frame, ret 14h.
// Source: GameCODA 2.10 CODAApiDbg.lib ReverbFns_SSE.obj CodeView (locals fData / pfData / i, statement and operand order).
// Retail-asm: 0x00C88FEA `mov esi,[ebx+0Ch]` .. 0x00C89001 `subps xmm0,xmm0` .. 0x00C890AA `dec ecx` .. 0x00C890B5 `movaps [edx+80h],xmm2`,
//   the SDK's inline SSE block, reached through the shared engine/ReverbFnsSSEFilterOrder2Quad.h
//   (CODA_SSE_APPLY_FILTER_ORDER2_QUAD, byte-matching path + FABLE_PORTABLE twin). The /Od body of the same function in CODAApiDbg.lib
//   carries the identical block (fixed registers, no /Od spills), so the original used asm; see the header for the evidence.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD
#include "engine/CReverbFnsSSE.h"
#include "engine/CFloatFilterParams.h"
#include "engine/ReverbFnsSSEFilterOrder2Quad.h"

void CReverbFnsSSE::ApplyFilterOrder2_quad(float *pfDest, const float *pfSrc, float **apHistory, CFloatFilterParams **aFilterParams, unsigned int uiSamples)
{
    __declspec(align(16)) float fData[40];
    float *pfData;
    int i;

    pfData = fData;
    for (i = 0; i < 4; i++)
    {
        fData[i] = aFilterParams[i]->m_fA[1];
        fData[i + 4] = aFilterParams[i]->m_fA[2];
        fData[i + 8] = 1.0f - (aFilterParams[i]->m_fA[1] + aFilterParams[i]->m_fA[2]);
        fData[i + 12] = aFilterParams[i]->m_fB[0];
        fData[i + 16] = aFilterParams[i]->m_fB[1];
        fData[i + 20] = aFilterParams[i]->m_fB[2];
        fData[i + 24] = -(aFilterParams[i]->m_fB[0] + aFilterParams[i]->m_fB[1] + aFilterParams[i]->m_fB[2]);
        fData[i + 28] = apHistory[i][0];
        fData[i + 32] = apHistory[i][1];
        fData[i + 36] = aFilterParams[i]->m_fGain;
    }

    CODA_SSE_APPLY_FILTER_ORDER2_QUAD(pfDest, pfSrc, uiSamples, pfData);

    for (i = 0; i < 4; i++)
    {
        apHistory[i][0] = fData[i + 28];
        apHistory[i][1] = fData[i + 32];
    }
}
