// Leaf: ?PreloadNewScene@CGameScriptInterface@@UBEXM@Z
// CGameScriptInterface_Methods::PreloadNewScene  @ 0x00890b70
// Reads a global manager, hops two pointers to reach a scene object,
// then makes a virtual call (vtable slot 0xdc/4 = 55) with args (4, param).
// this = the scene object (passed in ecx).

#define FABLE_RETAIL_NSCRIPT   // alias route: engine/CGameScriptInterface.h declares PreloadNewScene under this class-model switch
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its display engine (PDisplayEngine +0x28, GetDisplayEngine()) and that engine's Engine3D +0x2C
//   (Get3DEngine()), slot 55 CIEngine::StartNewScene(unsigned long, float) (engine/CIEngine.h); was the stand-in SceneRootHolder* g_sceneRoot with its SceneMidHolder / SceneTarget::PreloadScene stand-ins.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"

struct CGameScriptInterface_Methods : CGameScriptInterface {

};

void CGameScriptInterface::PreloadNewScene(float param) const
{
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().StartNewScene(4, param);
}