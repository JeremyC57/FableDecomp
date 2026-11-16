// CEngine::SetSpotlightShadowMode(bool enable) -- switch the lighting manager's shadow mode between spot lights (2) and the main light (1)
// Leaf: ?SetSpotlightShadowMode@CEngine@@UAEX_N@Z
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23C00: `this` (ecx) never read; writes (enable != 0) + 1 to [GPLightingManager + 0x4694].
// Identity: slot 62 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine); Ego_r.pdb + FableWin.pdb ?SetSpotlightShadowMode@CEngine@@UAEX_N@Z.
//   FableWin.pdb places ?SetShadowMode@CEngineLightingManager@@QAEXW4ELightingManagerShadowMode@@@Z (0x02C9E7C0) right after it (0x02C9E790):
//   the debug body calls it; retail inlines it.
// Global: 0x01436E9C, its landed spelling NEngineInternal::GPLightingManager (CEngineLightingManager*); ShadowMode is the shared
//   header's member at +0x4694 (ELightingManagerShadowMode: NULL 0, MAIN_LIGHT 1, SPOT_LIGHTS 2).
// Shared headers: engine/CEngine.h (FABLE_RETAIL_CENGINE + nested _SETSPOTLIGHTSHADOWMODE override declaration) and
//   engine/CEngineLightingManager.h (_MEMBERS + _SHADOWMODE_ENUM + nested _SETSHADOWMODE inline setter); replaces the local
//   CGlobalState / CEngine_Methods views and the made-up g_engineGlobal.
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_SETSPOTLIGHTSHADOWMODE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWMODE_ENUM
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETSHADOWMODE
#include "engine/CEngine.h"
#include "engine/CEngineLightingManager.h"

namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C

void CEngine::SetSpotlightShadowMode(bool enable)
{
    NEngineInternal::GPLightingManager->SetShadowMode(enable ? LIGHTING_MANAGER_SHADOW_MODE_SPOT_LIGHTS : LIGHTING_MANAGER_SHADOW_MODE_MAIN_LIGHT);
}
