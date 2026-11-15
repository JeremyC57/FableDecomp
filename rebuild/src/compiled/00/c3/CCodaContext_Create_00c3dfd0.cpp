// CCodaContext::Create [0x00C3DFD0] -- create the platform and each requested static source/listener/environment resource array.
// Grade: RELOCATION_MATCH -- whole642bytes plus matched EH code/data and original shared helper bodies.
// Leaf: ?Create@CCodaContext@@QAE_NPBU_CRESOURCEDESC@@PBUCPLATFORMDESC@@@Z
// Flags: /EHsc
// Retail: Fable.exe TLC @0x00C3DFD0,642bytes,ret8.
#define FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
#include "engine/CCodaContext.h"
#include "engine/CCoda2DSource.h"
#include "engine/CCoda3DSource.h"
#include "engine/CCodaListener.h"
#include "engine/CodaEnvironment.h"
#include "engine/CSalPlatform.h"

bool CCodaContext::Create(const CRESOURCEDESC* pcResources,const CPLATFORMDESC* pcPlatform)
{
    int nError=CSalContext::Create(pcResources,pcPlatform,&m_pSal);
    if(nError) {
        CODA.SetError(nError);
        return false;
    }
    if(pcResources) {
        m_nMax2DSources=pcResources->n2DSources;
        m_nMax3DSources=pcResources->n3DSources;
        m_nMaxListeners=pcResources->nListeners;
        m_nMaxEnvironments=pcResources->nEnvironments;
    }
    if(m_nMax2DSources>0) {
        m_2DSourceMem=new CCoda2DSource[m_nMax2DSources];
        if(!m_2DSourceMem)return false;
        m_2DSources.CreateStatic(m_2DSourceMem,sizeof(CCoda2DSource),m_nMax2DSources);
    }
    if(m_nMax3DSources>0) {
        m_3DSourceMem=new CCoda3DSource[m_nMax3DSources];
        if(!m_3DSourceMem)return false;
        m_3DSources.CreateStatic(m_3DSourceMem,sizeof(CCoda3DSource),m_nMax3DSources);
    }
    if(m_nMaxListeners>0) {
        m_ListenerMem=new CCodaListener[m_nMaxListeners];
        if(!m_ListenerMem)return false;
        m_Listeners.CreateStatic(m_ListenerMem,sizeof(CCodaListener),m_nMaxListeners);
    }
    if(m_nMaxEnvironments>0) {
        m_EnvironmentMem=new CCodaEnvironment[m_nMaxEnvironments];
        if(!m_EnvironmentMem)return false;
        m_Environments.CreateStatic(m_EnvironmentMem,sizeof(CCodaEnvironment),m_nMaxEnvironments);
    }
    m_EAX=0;
    return true;
}