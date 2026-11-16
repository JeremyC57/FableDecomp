// 00caa8d0 CBaseMediaFilter::GetSyncSource -- original parent for unwind01216e20
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?GetSyncSource@CBaseMediaFilter@@UAGJPAPAUIReferenceClock@@@Z -- 130 bytes verified against Fable.exe 0x00CAA8D0
// Flags: /EHsc
// Leaf: ?GetSyncSource@CBaseMediaFilter@@UAGJPAPAUIReferenceClock@@@Z
// Retail: parent00caa8d0,130bytes. SDK source and native EH/vtable evidence.
#define FABLE_RETAIL_CBASEMEDIAFILTER_PASS23
#include "rebuild_abi.h"
#include "engine/CBaseMediaFilter.h"
long __stdcall CBaseMediaFilter::GetSyncSource(IReferenceClock** pClock)
{
    CheckPointer(pClock,E_POINTER);
    CAutoLock cObjectLock(m_pLock);
    if(m_pClock)m_pClock->AddRef();
    *pClock=m_pClock;
    return NOERROR;
}