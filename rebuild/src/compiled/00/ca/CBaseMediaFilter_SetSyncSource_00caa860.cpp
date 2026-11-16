// 00caa860 CBaseMediaFilter::SetSyncSource -- original parent for unwind01216e00
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?SetSyncSource@CBaseMediaFilter@@UAGJPAUIReferenceClock@@@Z -- 109 bytes verified against Fable.exe 0x00CAA860
// Flags: /EHsc
// Leaf: ?SetSyncSource@CBaseMediaFilter@@UAGJPAUIReferenceClock@@@Z
// Retail: parent00caa860,109bytes. SDK source and native EH/vtable evidence.
#define FABLE_RETAIL_CBASEMEDIAFILTER_PASS23
#include "rebuild_abi.h"
#include "engine/CBaseMediaFilter.h"
long __stdcall CBaseMediaFilter::SetSyncSource(IReferenceClock* pClock)
{
    CAutoLock cObjectLock(m_pLock);
    if(pClock)pClock->AddRef();
    if(m_pClock)m_pClock->Release();
    m_pClock=pClock;
    return NOERROR;
}