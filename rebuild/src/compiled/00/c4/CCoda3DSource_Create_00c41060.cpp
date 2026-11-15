// CCoda3DSource::Create [0x00C41060] -- select HRTF mode, create the source, acquire an unlimited software voice when needed, then initialise the original 3D parameters.
// Grade: RELOCATION_MATCH -- 621bytes,9exact relocations; full4608case tests pass.
// Leaf: ?Create@CCoda3DSource@@UAE_NI@Z
// Retail: Fable.exe TLC @0x00C41060,621bytes,ret4.
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#include "engine/CCoda3DSource.h"
#include "engine/CCodaContext.h"
#include "engine/CSalPlatform.h"
#include "engine/CodaCaps.h"
#include <float.h>

bool CCoda3DSource::Create(unsigned int ffCaps)
{
    if(ffCaps&C_CAPS_3D_HRTF_FULL)
        m_HrtfMode=C_HRTF_FULL;
    else if(ffCaps&C_CAPS_3D_HRTF_DELAY)
        m_HrtfMode=C_HRTF_DELAY;
    else
        m_HrtfMode=static_cast<CHRTFMODE>(ffCaps&C_CAPS_3D_VOLPAN);
    if(!CCodaSource::Create(ffCaps))
        return false;
    if(!g_SalCaps.HW3DVoice.nFreeVoices || (ffCaps&~g_SalCaps.HW3DVoice.ffCaps)) {
        if(g_SalCaps.SW3DVoice.nFreeVoices==-1) {
            int error=CODA.GetSal().AllocateSw3DVoice(this,&m_pVoice);
            if(error) {
                CODA.SetError(error);
                Release();
                return false;
            }
            m_ffStatus|=0x20000;
        }
    }
    m_vPosition.Set(0.0f,0.0f,1.0f);
    m_vVelocity.Zero();
    m_vConeDirection.Set(0.0f,0.0f,1.0f);
    m_fMaxDistance=FLT_MAX;
    m_fMinDistance=1.0f;
    m_fRefDistance=1.0f;
    m_fRolloff=1.0f;
    m_fDoppler=1.0f;
    m_uiConeInner=360;
    m_uiConeOuter=360;
    m_fConeOuterGain=1.0f;
    m_iDirect=0;
    m_iDirectHF=0;
    if(ffCaps&C_CAPS_REVERB) {
        m_EnvFx.iEnvironment=0;
        m_EnvFx.iEnvironmentHF=0;
        m_EnvFx.iObstruction=0;
        m_EnvFx.fObstructionLFRatio=0.0f;
        m_EnvFx.iOcclusion=0;
        m_EnvFx.fOcclusionLFRatio=0.0f;
        m_EnvFx.iExclusion=0;
        m_EnvFx.fExclusionLFRatio=0.0f;
        m_EnvFx.fRolloffFactor=1.0f;
        m_EnvFx.fAirAbsorptionFactor=1.0f;
    }
    m_pfnDistanceCallback=0;
    m_DistanceCallbackData=0;
    m_3D.vPosition=m_vPosition;
    m_3D.vVelocity=m_vVelocity;
    m_3D.vConeDirection=m_vConeDirection;
    m_3D.vDirection=m_vPosition;
    m_3D.fDistance=1.0f;
    m_3D.fDistanceGain=1.0f;
    m_3D.fConeGain=1.0f;
    m_3D.fDoppler=1.0f;
    m_3D.fMacroFx=1.0f;
    m_3D.iDirect=0;
    m_3D.iDirectHF=0;
    m_3D.uiDirectHFReference=CODA.GetHFReference();
    m_3D.iEnvFx=(ffCaps&C_CAPS_REVERB)?0:-10000;
    m_3D.iEnvFxHF=0;
    m_3D.uiEnvFxHFReference=5000;
    m_ffCommit=0;
    return true;
}