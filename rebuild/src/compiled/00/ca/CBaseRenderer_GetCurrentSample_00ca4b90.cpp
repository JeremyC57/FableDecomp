// CBaseRenderer::GetCurrentSample() -- under the renderer lock, AddRef and return the held media sample (DirectShow BaseClasses)
// Leaf: ?GetCurrentSample@CBaseRenderer@@UAEPAUIMediaSample@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4B90, 93 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::GetCurrentSample is primary-vtable slot 41; retail CBaseRenderer vtable 0x012BF498 slot 41 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 41 hold 0x00CA4B90.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CGuiWindowManager::RemoveExclusiveForegroundWindow() -- EH-framed: holds the critical section at +0x94, adds a reference to the foreground window at +0x6c (COM-style `__stdcall` slot 1) when present a (Leaf none, module CGuiWindowManager).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
IMediaSample* CBaseRenderer::GetCurrentSample()
{
    CAutoLock cRendererLock(&m_RendererLock);
    if (m_pMediaSample) {
        m_pMediaSample->AddRef();
    }
    return m_pMediaSample;
}
