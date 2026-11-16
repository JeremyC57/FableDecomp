// 00c74510 CFloatFilter::ApplyFilter
// Leaf: ?ApplyFilter@CFloatFilter@@AAEXPBM0PAM01_NI@Z
// Flags: none
// Retail: Fable.exe00c74510,828bytes,ret1ch; SDK release PUBLIC/debug full CodeView original name/signature.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#include "engine/CFloatFilter.h"
#include <string.h>
#pragma intrinsic(memcpy)
void CFloatFilter::ApplyFilter(const float* pfA, const float* pfB, float* pfZ, const float* pfSrc, float* pfDest,
    bool bInputValid, unsigned int uiSamples)
{
    float fA1 = pfA[1];
    float fA2 = pfA[2];
    float fB0 = pfB[0];
    float fB1 = pfB[1];
    float fB2 = pfB[2];
    if (fA2 == 0.0f && fB2 == 0.0f) {
        float fOffsetCorrectionA = 1.0f - fA1;
        float fOffsetCorrectionB = -(fB1 + fB0);
        if (bInputValid) {
            if (fA1 == 0.0f && fB1 == 0.0f) {
                if (fB0 == 1.0f) {
                    if (pfDest != pfSrc)
                        memcpy(pfDest, pfSrc, uiSamples * sizeof(float));
                } else {
                    m_pDSP->ScaleSamples(pfDest, pfSrc, fB0, uiSamples);
                }
                pfZ[0] = pfDest[uiSamples - 1];
            } else {
                float z0 = pfZ[0];
                for (unsigned int i = 0; i < uiSamples; i++) {
                    float fTemp = fB1 * z0;
                    z0 = z0 * fA1 + *pfSrc++ + fOffsetCorrectionA;
                    *pfDest++ = fB0 * z0 + fTemp + fOffsetCorrectionB;
                }
                pfZ[0] = z0;
            }
        } else {
            float z0 = pfZ[0];
            for (unsigned int i = 0; i < uiSamples; i++) {
                float fTemp = fB1 * z0;
                z0 = z0 * fA1 + fOffsetCorrectionA;
                *pfDest++ = fB0 * z0 + fTemp + fOffsetCorrectionB;
            }
            pfZ[0] = z0;
        }
    } else if (bInputValid) {
        m_pDSP->ApplyFilterOrder2(pfDest, pfSrc, pfA, pfB, pfZ, uiSamples);
    } else {
        float fOffsetCorrectionA=1.0f-(fA1+fA2);
        float fOffsetCorrectionB=-(fB0+fB1+fB2);
        float z0=pfZ[0];
        float z1=pfZ[1];
        for(unsigned int i=0;i<uiSamples;i++) {
            float z2=z1;
            z1=z0;
            *pfDest++=(z0=z1*fA1+z2*fA2+fOffsetCorrectionA)*fB0+z1*fB1+z2*fB2+fOffsetCorrectionB;
        }
        pfZ[0]=z0;
        pfZ[1]=z1;
    }
}