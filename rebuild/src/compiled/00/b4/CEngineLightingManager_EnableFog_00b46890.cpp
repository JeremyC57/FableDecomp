// CEngineLightingManager::EnableFog() -- requests render state "fog enable" = 1 through the global render-state manager (captured for undo once per capture mask, queued for realisation once)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B46890, length 109 bytes. No calls.
// Globals: the render-state manager @ 0x01436E18 (the same tracked-state record family that the landed
// CStateBlockFunctionSold::Apply @ 0x009DF060 drives: capture stack of {state, prior value} pairs at +0x08, dirty-state
// list at +0x2008, capture count +0x2808, dirty count +0x280C, active capture mask +0x2814, 0x14-byte tracked states).
// Shape: one inline `SetState(state, value)` on the fog-enable record at +0x2944 (its address formed once with `lea`):
// nothing when the requested value already matches; otherwise, unless the state's capture mask already holds the active
// mask, OR it in and push {state, old value} on the capture stack (the count re-read for each store), then store the
// value and, if not yet queued, flag it and append it to the dirty list. `this` is never read (`ret`). SetState must be
// `__forceinline` (plain inline stays out of line); the mask test is written `(m_captureMask & state.m_captureMask) == 0`
// with `|=` (a cached `mask` local swaps edx/esi).
#include "fable_compiler.h"
// NF45 (round 7): the manager is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A; engine/CRenderManager.h, CRenderStateManager at offset 0); the fog-enable
//   record +0x2944 is DeviceState.FogEnable (+0x2818 + 0x12C), set by the shared inline CRenderStateManager::SetFogEnable(true)
//   (was a local CRenderStateManager model and g_pRenderStateManager_01436e18).
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

class CEngineLightingManager { public:
    void EnableFog();
};

void CEngineLightingManager::EnableFog()
{
    NEngineInternal::GPRenderManager->SetFogEnable(true);
}