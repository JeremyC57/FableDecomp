// CEngineWaterRenderer::WorldFrameUpdate(bool force) -- advance the water settings' per-world-frame animation (swell, wake and bump-map offsets)
// Leaf: ?WorldFrameUpdate@CEngineWaterRenderer@@UAEX_N@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py with FABLE_HDR_OVERLAY=hdr/, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D6D0, length 11 bytes: `add ecx,280h ; jmp 0x00BE95F0` (the bool argument stays
//   on the stack for the callee's `ret 4`).
// Identity: slot 7 of the CEngineWaterRenderer vtable 0x012A3364 (RTTI CEngineWaterRenderer : CEngineComponent); Ego_r.pdb and
//   FableWin.pdb ?WorldFrameUpdate@CEngineWaterRenderer@@UAEX_N@Z. Catalog row had no name ("CEngineWaterRenderer::WorldFrameUpdate"
//   only in the chunk list).
// Callee: CEngineWaterSettings::WorldFrameUpdate(bool) @ 0x00BE95F0 (Ego_r + FableWin ?WorldFrameUpdate@CEngineWaterSettings@@QAEX_N@Z;
//   it tests the bool, then the weather flag, and steps the shift histories), on the embedded Settings at +0x280.
// Header: needs the opt-in branches FABLE_RETAIL_CENGINEWATERRENDERER (engine/CEngineWaterRenderer.h, polymorphic on the retail
//   CEngineComponent of FABLE_RETAIL_CENGINECOMPONENT) and the new engine/CEngineWaterSettings.h (hand-back hdr/).
#define FABLE_RETAIL_CENGINEWATERRENDERER
#include "engine/CEngineWaterRenderer.h"

void CEngineWaterRenderer::WorldFrameUpdate(bool force)
{
    Settings.WorldFrameUpdate(force);
}