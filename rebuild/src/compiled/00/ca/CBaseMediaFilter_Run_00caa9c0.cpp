// 00caa9c0 CBaseMediaFilter::Run -- original parent for unwind01216e40
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?Run@CBaseMediaFilter@@UAGJ_J@Z -- 148 bytes verified against Fable.exe 0x00CAA9C0
// Flags: /EHsc
// Leaf: ?Run@CBaseMediaFilter@@UAGJ_J@Z
// Retail: parent00caa9c0,148bytes. SDK source and native EH/vtable evidence.
#define FABLE_RETAIL_CBASEMEDIAFILTER_PASS23
#include "rebuild_abi.h"
#include "engine/CBaseMediaFilter.h"
long __stdcall CBaseMediaFilter::Run(REFERENCE_TIME tStart)
{
    CAutoLock cObjectLock(m_pLock);
    m_tStart=tStart;
    if(m_State==State_Stopped){
        long hr=Pause();
        if(hr<0)return hr;
    }
    m_State=State_Running;
    return S_OK;
}