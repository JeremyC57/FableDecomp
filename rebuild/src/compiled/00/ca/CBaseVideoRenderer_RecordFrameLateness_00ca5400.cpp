// CBaseVideoRenderer::RecordFrameLateness(int, int) -- accumulate frame lateness and inter-frame statistics (clamped to +-1 s; the first frames ignored) (DirectShow BaseClasses)
// Leaf: ?RecordFrameLateness@CBaseVideoRenderer@@UAEXHH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA5400, 262 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h: CBaseVideoRenderer overrides RecordFrameLateness (primary slot 45); retail CBaseVideoRenderer vtable 0x012BF5D8 slot 45 (its ??_G 0x00CA73D0) holds 0x00CA5400, CBaseRenderer vtable 0x012BF498 slot 45 holds -.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CBaseVideoRenderer::RecordFrameLateness(int, int) -- DirectShow base-class quality statistics: accumulates the clamped lateness and inter-frame time (ms) and counts the frame (Leaf RecordFrameLateness, module CBaseVideoRenderer).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
#define FRAMES_TO_IGNORE 1
void CBaseVideoRenderer::RecordFrameLateness(int trLate, int trFrame)
{
    // Record how timely we are.
    int tLate = trLate / 10000;
    // Ignore frames more than a second off (they invalidate the statistics).
    if (tLate > 1000 || tLate < -1000) {
        if (m_cFramesDrawn <= FRAMES_TO_IGNORE) {
            tLate = 0;
        } else if (tLate > 0) {
            tLate = 1000;
        } else {
            tLate = -1000;
        }
    }
    // The very first frame often has an invalid time.
    if (m_cFramesDrawn > FRAMES_TO_IGNORE) {
        m_iTotAcc += tLate;
        m_iSumSqAcc += (tLate * tLate);
    }
    // Inter-frame time makes no sense for the first two frames.
    if (m_cFramesDrawn > FRAMES_TO_IGNORE + 1) {
        int tInterFrame = trFrame / 10000;
        // Clamp the time to 1 second.
        if (tInterFrame > 1000 || tInterFrame < 0) tInterFrame = 1000;
        m_iSumSqFrameTime += tInterFrame * tInterFrame;
        m_iSumFrameTime += tInterFrame;
    }
    ++m_cFramesDrawn;
}
