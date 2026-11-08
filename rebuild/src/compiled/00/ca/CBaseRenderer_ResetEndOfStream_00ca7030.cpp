// CBaseRenderer::ResetEndOfStream() -- reset the end-of-stream state: kill the EOS timer (inline ResetEndOfStreamTimer), then under the renderer lock clear m_bEOS / m_bEOSDelivered / m_SignalTime (DirectShow BaseClasses)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?ResetEndOfStream@CBaseRenderer@@UAEJXZ -- 74 bytes verified against Fable.exe 0x00CA7030
// Leaf: ?ResetEndOfStream@CBaseRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA7030, 74 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::ResetEndOfStream is primary-vtable slot 25; retail CBaseRenderer vtable 0x012BF498 slot 25 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 25 hold 0x00CA7030.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CTimedQueue_00ca7030::Reset() -- kill the multimedia timer at +0xc0 (timeKillEvent @ IAT 0x014404B0) when set, then under the critical section at +0x94 (Enter/LeaveCriticalSection @ IAT 0x0143FE0C / 0 (Leaf none, module CTimedQueue_00ca7030).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
extern "C" __declspec(dllimport) unsigned int __stdcall timeKillEvent(unsigned int uTimerID);   // WINMM import 0x014404B0
// renbase.cpp: the timer reset is a separate member that this TU inlines.
void CBaseRenderer::ResetEndOfStreamTimer()
{
    if (m_EndOfStreamTimer) {
        timeKillEvent(m_EndOfStreamTimer);
        m_EndOfStreamTimer = 0;
    }
}
long CBaseRenderer::ResetEndOfStream()
{
    ResetEndOfStreamTimer();
    CAutoLock cRendererLock(&m_RendererLock);
    m_bEOS = 0;
    m_bEOSDelivered = 0;
    m_SignalTime = 0;
    return NOERROR;
}
