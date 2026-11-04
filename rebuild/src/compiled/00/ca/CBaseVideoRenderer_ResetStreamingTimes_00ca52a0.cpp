// CBaseVideoRenderer::ResetStreamingTimes() -- reset every quality-management statistic for a new streaming run (DirectShow BaseClasses)
// Leaf: ?ResetStreamingTimes@CBaseVideoRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA52A0, 191 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h: CBaseVideoRenderer overrides ResetStreamingTimes (primary slot 47); retail CBaseVideoRenderer vtable 0x012BF5D8 slot 47 (its ??_G 0x00CA73D0) holds 0x00CA52A0, CBaseRenderer vtable 0x012BF498 slot 47 holds -.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CBaseVideoRenderer::ResetStreamingTimes() -- DirectShow base-class quality statistics reset (renbase.cpp), returns NOERROR (Leaf ResetStreamingTimes, module CTextureRenderer).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);   // WINMM import 0x014404B4
long CBaseVideoRenderer::ResetStreamingTimes()
{
    m_trLastDraw = -1000;     // set up as first frame since ages (1 sec) ago
    m_tStreamingStart = timeGetTime();
    m_trRenderAvg = 0;
    m_trFrameAvg = -1;        // -1000 fps == "unset"
    m_trDuration = 0;         // 0 - strange value
    m_trRenderLast = 0;
    m_trWaitAvg = 0;
    m_tRenderStart = 0;
    m_cFramesDrawn = 0;
    m_cFramesDropped = 0;
    m_iTotAcc = 0;
    m_iSumSqAcc = 0;
    m_iSumSqFrameTime = 0;
    m_trFrame = 0;            // hygiene - not really needed
    m_trLate = 0;             // hygiene - not really needed
    m_iSumFrameTime = 0;
    m_nNormal = 0;
    m_trEarliness = 0;
    m_trTarget = -300000;     // 30mSec early
    m_trThrottle = 0;
    m_trRememberStampForPerf = 0;
    return NOERROR;
}
