#define FABLE_RETAIL_REVERBMATH_ORIGINAL_DS_FTOI_PASS26
// Original GameCODA2.10 CodeView and native CreateComponents calls prove class/API and all names.
// Leaf: ?Create@CFloatEcho@@QAEXIAAPAXPAHH@Z
// Flags: (none)
// Retail-asm: 00C76ADB fistp dword ptr [esp+8]; original ds_ftoi uses bare dword conversion, shared LIBPK_FTOL portable twin.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATECHO_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#include "engine/CFloatEcho.h"
#include "engine/ReverbMath.h"
#pragma intrinsic(memset)
void CFloatEcho::Create(unsigned int uiSampleRate,void*& pvStaticData,int* piSizeOfStaticData,int iFaderToken)
{
    unsigned int iNumSamples=(ds_ftoi(uiSampleRate*0.25f)+3)&~3;
    if (pvStaticData) {
        m_buffer.Create((float*)pvStaticData,iNumSamples);
        m_buffer.Clear();
        m_iMyFaderToken=iFaderToken;
        pvStaticData=(float*)pvStaticData+iNumSamples;
    }
    *piSizeOfStaticData+=iNumSamples*sizeof(float);
}