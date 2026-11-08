// CBaseRenderer::StartStreaming() -- start streaming under the renderer lock: timeBeginPeriod(1), OnStartStreaming, then deliver EOS or schedule the held sample (DirectShow BaseClasses)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?StartStreaming@CBaseRenderer@@UAEJXZ -- 166 bytes verified against Fable.exe 0x00CA4D80
// Leaf: ?StartStreaming@CBaseRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4D80, 166 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::StartStreaming is primary-vtable slot 31; retail CBaseRenderer vtable 0x012BF498 slot 31 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 31 hold 0x00CA4D80.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CAudioDevice_00ca4d80::Start() -- EH-framed: holds the critical section at +0x94 through a scoped lock (state 0); when the state at +0x64 is not 1 it becomes 1, the import @ IAT 0x014404AC is called w (Leaf none, module _unattributed).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
extern "C" __declspec(dllimport) unsigned int __stdcall timeBeginPeriod(unsigned int uPeriod);   // WINMM import 0x014404AC
long CBaseRenderer::StartStreaming()
{
    CAutoLock cRendererLock(&m_RendererLock);
    if (m_bStreaming == 1) {
        return NOERROR;
    }
    // Reset the streaming times ready for running
    m_bStreaming = 1;
    timeBeginPeriod(1);
    OnStartStreaming();
    // If we have an EOS and no data then deliver it now
    if (m_pMediaSample == 0) {
        return SendEndOfStream();
    }
    // Have the data rendered
    if (!ScheduleSample(m_pMediaSample))
        m_RenderEvent.Set();
    return NOERROR;
}
