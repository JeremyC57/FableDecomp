// 00c6b050 CFloatReverbEngine::Create
// Flags: none
// CFloatReverbEngine::Create [0x00C6B050] -- fill default reverb buffer lengths, configure the translator/DSP engine, and commit the default high-level parameters.
// Grade: RELOCATION_MATCH -- retail_asm via existing shared FtoL;490bytes,19exact relocations.
// Leaf: ?Create@CFloatReverbEngine@@QAE_NPAVCReverbConstants@@@Z
// Retail: Fable.exe TLC @0x00C6B050,490bytes,ret4.
// Retail-asm: existing shared libpk/FtoL.h at 0x00C6B08E, 0x00C6B0CF, 0x00C6B10A, 0x00C6B14B, 0x00C6B1A2, 0x00C6B212; bare `fistp dword ptr [esp+10h]`. Current per-site C++/intrinsic ladder: rounding_ladder/RESULTS.json,336forms plus12actual member-store forms (rounding_member/RESULTS.json),none emitted it.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#include "engine/CFloatReverbEngine.h"
#include "engine/CHLRvbParams.h"
#include "libpk/FtoL.h"
#include <string.h>

bool CFloatReverbEngine::Create(CReverbConstants* pConsts)
{
    if (!pConsts->m_uiMaxReflectionsInputSamples) {
        long n=FtoL(pConsts->m_uiSamplesPerSecond * 0.5404f - 0.5f);
        if(n<0)++n;
        pConsts->m_uiMaxReflectionsInputSamples=n;
    }
    if (!pConsts->m_uiMaxShortReverbInputSamples) {
        long n=FtoL(pConsts->m_uiSamplesPerSecond * 0.1f - 0.5f);
        if(n<0)++n;
        pConsts->m_uiMaxShortReverbInputSamples=n;
    }
    if (!pConsts->m_uiMaxLongReverbInputSamples) {
        long n=FtoL((float)pConsts->m_uiSamplesPerSecond - 0.5f);
        if(n<0)++n;
        pConsts->m_uiMaxLongReverbInputSamples=n;
    }
    if (!pConsts->m_uiMaxModulationSamples) {
        long n=FtoL(pConsts->m_uiSamplesPerSecond * 0.5f - 0.5f);
        if(n<0)++n;
        pConsts->m_uiMaxModulationSamples=n;
    }
    memcpy(&m_RvbConsts,pConsts,sizeof(CReverbConstants));
    static const float sc_fDefaultShortReverbBufferLength[4]={0.048649f,0.058005f,0.070295f,0.085238f};
    for(unsigned int i=0;i<4;++i) {
        long n=FtoL(m_RvbConsts.m_uiSamplesPerSecond * sc_fDefaultShortReverbBufferLength[i] - 0.5f);
        if(n<0)++n;
        m_RvbConsts.m_ShortRvbConstants[i].uiBufferLength=n;
    }
    m_Translator.Create(&m_RvbConsts);
    bool bCreated=m_DspEngine.Create(&m_RvbConsts);
    CHLRvbParams params;
    m_Translator.Translate(&m_RvbParams,&params);
    CReverbDspEngine* pEngine=&m_DspEngine;
    pEngine->Commit(&m_RvbParams,true);
    long n=FtoL(params.GetDecayTime() - 0.5f);
    if(n<0)++n;
    m_iDecayTimeSamples=m_RvbConsts.m_uiSamplesPerSecond * n;
    return bCreated;
}