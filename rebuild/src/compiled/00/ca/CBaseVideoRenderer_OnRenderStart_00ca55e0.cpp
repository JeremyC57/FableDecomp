// CBaseVideoRenderer::OnRenderStart(struct IMediaSample*) -- record the previous frame's lateness, then stamp the render start time (DirectShow BaseClasses)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?OnRenderStart@CBaseVideoRenderer@@UAEXPAUIMediaSample@@@Z -- 43 bytes verified against Fable.exe 0x00CA55E0
// Leaf: ?OnRenderStart@CBaseVideoRenderer@@UAEXPAUIMediaSample@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA55E0, 43 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h: CBaseVideoRenderer overrides OnRenderStart (primary slot 14); retail CBaseVideoRenderer vtable 0x012BF5D8 slot 14 (its ??_G 0x00CA73D0) holds 0x00CA55E0, CBaseRenderer vtable 0x012BF498 slot 14 holds 0x00CA4F20.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CBaseVideoRenderer::OnRenderStart(IMediaSample*) -- the DirectShow base-class member: record the (Leaf none, module CBaseVideoRenderer).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);   // WINMM import 0x014404B4
void CBaseVideoRenderer::OnRenderStart(IMediaSample* pMediaSample)
{
    RecordFrameLateness(m_trLate, m_trFrame);
    m_tRenderStart = timeGetTime();
}
