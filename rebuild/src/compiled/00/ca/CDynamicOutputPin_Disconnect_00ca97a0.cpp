// 00ca97a0 CDynamicOutputPin::Disconnect
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?Disconnect@CDynamicOutputPin@@UAGJXZ -- 127 bytes verified against Fable.exe 0x00CA97A0
// Flags: /EHsc
// Leaf: ?Disconnect@CDynamicOutputPin@@UAGJXZ
// Original SDK amfilter.cpp body; native IPin vtable slot5 and EH parent verified.
#define FABLE_RETAIL_CDYNAMICOUTPUTPIN_PASS23
#include "rebuild_abi.h"
#include "engine/CDynamicOutputPin.h"
long __stdcall CBasePin::DisconnectInternal()
{
    if(m_Connected) {
        long hr=BreakConnect();
        if(hr<0)return hr;
        m_Connected->Release();
        m_Connected=0;
        return S_OK;
    } else {
        return S_FALSE;
    }
}
long __stdcall CDynamicOutputPin::Disconnect()
{
    CAutoLock cObjectLock(m_pLock);
    return DisconnectInternal();
}