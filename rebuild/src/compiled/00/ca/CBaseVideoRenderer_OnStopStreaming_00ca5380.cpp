// CBaseVideoRenderer::OnStopStreaming() -- end of streaming: m_tStreamingStart becomes the elapsed streaming time (timeGetTime() - start) (DirectShow BaseClasses)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?OnStopStreaming@CBaseVideoRenderer@@UAEJXZ -- 25 bytes verified against Fable.exe 0x00CA5380
// Leaf: ?OnStopStreaming@CBaseVideoRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA5380, 25 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h: CBaseVideoRenderer overrides OnStopStreaming (primary slot 17); retail CBaseVideoRenderer vtable 0x012BF5D8 slot 17 (its ??_G 0x00CA73D0) holds 0x00CA5380, CBaseRenderer vtable 0x012BF498 slot 17 holds 0x00CA6310.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CBaseVideoRenderer::OnStopStreaming() -- DirectShow base-class hook: turns the streaming start time into the elapsed streaming time, returns NOERROR (Leaf none, module CBaseVideoRenderer).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);   // WINMM import 0x014404B4
long CBaseVideoRenderer::OnStopStreaming()
{
    m_tStreamingStart = timeGetTime() - m_tStreamingStart;
    return NOERROR;
}
