#define FABLE_RETAIL_REVERBMATH_ORIGINAL_DS_FTOI_PASS26
// Original GameCODA2.10 CodeView and native CreateComponents calls prove class/API and all names.
// Leaf: ?Create@CFloatPitch@@QAEXIAAPAXPAHH@Z
// Flags: (none)
// Retail-asm: 00C76DFB fistp dword ptr [esp+8]; original ds_ftoi uses bare dword conversion, shared LIBPK_FTOL portable twin.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATPITCH_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#include "engine/CFloatPitch.h"
#include "engine/ReverbMath.h"
#pragma intrinsic(memset)
void CFloatPitch::Create(unsigned int uiSamplesPerSecond,void*& pvStaticData,int* piSizeOfStaticData,int iFaderToken)
{
    unsigned int iNumSamples=ds_ftoi(uiSamplesPerSecond*0.5f);
    unsigned int uiBufferLength=(iNumSamples+3)&~3;
    if (pvStaticData) {
        m_Buffer.Create((float*)pvStaticData,uiBufferLength);
        m_Buffer.Clear();
        m_iMyFaderToken=iFaderToken;
        m_fControlPoint=0;
        pvStaticData=(float*)pvStaticData+uiBufferLength;
    }
    *piSizeOfStaticData+=uiBufferLength*sizeof(float);
}