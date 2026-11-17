// CEngineLightingManager::UpdateShadowScene(const CEngineCamera& camera) -- hand the camera to the shadow renderer with the current shadowed-spotlight parameters: shadow mode 2 as a shadowed pass (true), mode 1 as a plain pass (false), other modes do nothing
// Leaf: ?UpdateShadowScene@CEngineLightingManager@@QAEXABVCEngineCamera@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B444F0, length 80 bytes; thiscall `ret 4` on both arms. Its one retail call site 0x00B32A49
//   is not landed; CEngineLightingManager::FrameStart 0x00B484B0 expands the same inline body.
// Identity (agentNF57 name fix; was the stand-in ?UpdateShadowScene@CEngineLightingManager@@QAEXPAX@Z over a local by-address model):
//   Ego_r.pdb 0x009B9350 / FableWin.pdb 0x02CEDD40 ?UpdateShadowScene@CEngineLightingManager@@QAEXABVCEngineCamera@@@Z. The callee
//   is NF53's ?UpdateShadowScene@CEngineShadowRenderer@@QAEXABVCEngineCamera@@_NABUCEngineLightingManagerSpotlightParams@@@Z (0x00B4F390;
//   Ego_r.pdb 0x009F2070 / FableWin.pdb 0x02D77720), which the landed FrameStart 0x00B484B0 already binds.
// Shape: the inline body of the landed FrameStart 0x00B484B0 (ShadowMode +0x4694 read once, CurrentShadowedSpotLightParams +0x4698 by
//   reference), out of line, on engine/CEngineLightingManager.h (_MEMBERS; _FRAMESTART declares UpdateShadowScene) and engine/CEngineShadowRenderer.h (_UPDATESCENE).
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_FRAMESTART
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESCENE
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineCamera.h"
namespace NEngineInternal {
 extern CEngineShadowRenderer* GPShadowRenderer; // 0x01436E60
}

void CEngineLightingManager::UpdateShadowScene(const CEngineCamera& camera)
{
 if(ShadowMode==(ELightingManagerShadowMode)2)
  NEngineInternal::GPShadowRenderer->UpdateShadowScene(camera,true,CurrentShadowedSpotLightParams);
 else if(ShadowMode==(ELightingManagerShadowMode)1)
  NEngineInternal::GPShadowRenderer->UpdateShadowScene(camera,false,CurrentShadowedSpotLightParams);
}
