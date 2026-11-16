// CDisplayEngine::Render(const CInterpolationInfo* info, bool world_updated, bool render) -- the public entry outside callers use to
// render a frame: hands its three arguments straight to the protected DoRender
// Leaf: ?Render@CDisplayEngine@@QAEXPBVCInterpolationInfo@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00435F70, 5 bytes: `jmp DoRender` (0x00435530; the thiscall forward keeps ecx and the three stack
// arguments in place, so VC7.1 emits a sibling jump).
// Identity: Ego_r.pdb ?DoRender@CDisplayEngine@@IAEXPBVCInterpolationInfo@@_N1@Z 0x004490E0 then ?Render@...@@QAEXPBVCInterpolationInfo@@_N1@Z
//   0x00449BC0 (0xAE0 later; retail 0x00435530 then 0x00435F70, 0xA40 later); FableWin.pdb Render 0x01ADC6C0 (0x30 bytes, a forward)
//   right before DoRender 0x01ADC6F0. Callers: CUserProfileManager::CaptureBackBufferForUIBackground 0x00406CC0 (twice),
//   CMainGameComponent::UpdateForcedTickSpeed 0x00417747. (Landed earlier as a placeholder forwarder spelled DoRender; respelled
//   2026-11-16 once 0x00435530 landed as the real DoRender.)
#define FABLE_RETAIL_CDISPLAYENGINE_DORENDER
#include "engine/CDisplayEngine.h"

void CDisplayEngine::Render(const CInterpolationInfo* info, bool world_updated, bool render)
{
    DoRender(info, world_updated, render);
}
