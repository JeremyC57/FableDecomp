// 00c40d10 CCodaEnvironment::Commit
// Leaf: ?Commit@CCodaEnvironment@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CCODACONTEXT_PROCESS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#define FABLE_RETAIL_CCODAVECTOR_ENV_SCALE_PASS29
#include "engine/CCodaVector.h"
#include "engine/CSalOpenALReverb.h"

void CCodaEnvironment::Commit()
{
    if (m_ffCommit) {
        CCodaVector v;
        v=m_Params.vReflectionsPan;
        float fMagnitude=v.Magnitude();
        if (fMagnitude>1.0f) {
            v.Scale(1.0f/fMagnitude);
            m_Params.vReflectionsPan=v;
        }
        v=m_Params.vReverbPan;
        fMagnitude=v.Magnitude();
        if (fMagnitude>1.0f) {
            v.Scale(1.0f/fMagnitude);
            m_Params.vReverbPan=v;
        }
        m_pReverb->Update(this);
        m_ffCommit=0;
    }
}