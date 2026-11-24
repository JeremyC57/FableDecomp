// CFloatFilterParams::Set_ShelfLfHf(float,float,float,float,unsigned int) -- derive cascaded low/high shelf coefficients.
// Leaf: ?Set_ShelfLfHf@CFloatFilterParams@@QAEXMMMMI@Z
// Flags: (none)
// Evidence: GameCODA2.10 release FloatFilter.obj identity; debug CodeView exact parameters and scoped locals.
#include <math.h>
#pragma intrinsic(exp,sqrt)
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#include "engine/CFloatFilterParams.h"
const float _gc_fPI=3.14159274f;
void CFloatFilterParams::Set_ShelfLfHf(float fLFRatio,float fFreqLF,float fHFRatio,float fFreqHF,unsigned int uiSamplesPerSecond)
{
    if(fLFRatio>50.0f)fLFRatio=50.0f;
    if(fHFRatio>50.0f)fHFRatio=50.0f;
    if(fLFRatio<0.00001)fLFRatio=0.00001f;
    if(fHFRatio<0.00001)fHFRatio=0.00001f;
    float fLF_A1,fLF_B1,fHF_A1,fHF_B1;
    float norm=1.0f;
    float fFreqRef=sqrtf(fFreqLF*fFreqHF);
    if(fLFRatio==1.0f) {
        float fFreqCentre=sqrtf(fFreqRef*fFreqLF);
        float expf1=expf(-_gc_fPI*fFreqCentre/uiSamplesPerSecond);
        fLF_B1=fLF_A1=expf1;
    } else {
        float fSqrtGain=sqrtf(fLFRatio);
        float fFreqCentre=sqrtf(fFreqRef*fFreqLF);
        float fFilterF1=fFreqCentre/fSqrtGain;
        float fFilterF2=fFreqCentre*fSqrtGain;
        float expf1=expf(-_gc_fPI*fFilterF1/uiSamplesPerSecond);
        float expf2=expf(-_gc_fPI*fFilterF2/uiSamplesPerSecond);
        fLF_A1=expf1;
        fLF_B1=expf2;
    }
    if(fHFRatio==1.0f) {
        float fFreqCentre=sqrtf(fFreqRef*fFreqHF);
        float expf1=expf(-_gc_fPI*fFreqCentre/uiSamplesPerSecond);
        fHF_B1=fHF_A1=expf1;
    } else {
        float fSqrtGain=sqrtf(fHFRatio);
        float fFreqCentre=sqrtf(fFreqRef*fFreqHF);
        float fFilterF1=fFreqCentre*fSqrtGain;
        float fFilterF2=fFreqCentre/fSqrtGain;
        float expf1=expf(-_gc_fPI*fFilterF1/uiSamplesPerSecond);
        float expf2=expf(-_gc_fPI*fFilterF2/uiSamplesPerSecond);
        norm*=-(1.0f-expf1)/(expf2-1.0f);
        fHF_A1=expf1;
        fHF_B1=expf2;
    }
    m_fA[0]=1.0f;
    m_fA[1]=fLF_A1+fHF_A1;
    m_fA[2]=-fLF_A1*fHF_A1;
    m_fB[0]=1.0f*norm;
    m_fB[1]=-(fLF_B1+fHF_B1)*norm;
    m_fB[2]=fLF_B1*fHF_B1*norm;
}