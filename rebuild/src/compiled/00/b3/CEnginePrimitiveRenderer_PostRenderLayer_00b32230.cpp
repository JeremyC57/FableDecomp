// CEnginePrimitiveRenderer::PostRenderLayer -- reset current layer, normalize-normals tracked state, lights/world transform, then saved shader enable flags. Retail00b32230 complete207bytes+1int3. Shared renderer/state/lighting headers; donor leaves renderer saved layer+78 and shaderflags+260/+261 opaque. DeviceState NormalizeNormals at+230 (manager2a48), nativeCStateInfo20bytes and statecapture8bytes. CalleesB44D50/B32100/988110/988190/987FE0/9880E0.
#ifndef _MSC_VER
#define __forceinline inline
class CEnginePrimitiveRenderer { public:unsigned char _pad_0x78[sizeof(long)];unsigned char _pad_0x260[4];};
class CEngineLightingManager { public:};
struct CStateInfo;
class CRenderStateManager { public:unsigned char StateStack[32768];CStateInfo* StateUpdateList[256];long StateStackSize,StateUpdateListSize;unsigned long CurrentBookmarkMask;unsigned char DeviceState[16384];};
#else
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CRenderStateManager.h"
#include "engine/CEngineLightingManager.h"
#endif
struct CStateInfo {unsigned long CurrentState,DesiredState,BookmarkMask,DeviceStateID;bool DirtyListFlag;unsigned char StateType,TextureStage;};
struct CStateStackElement {CStateInfo* State;unsigned long Value;};
struct CDeviceStateNormalizeView {unsigned char EarlierStates[0x230];CStateInfo NormalizeNormals;};
class CShaderRenderManager { public:unsigned char EarlierMembers[0x390];bool VertexShadersEnabled,PixelShadersEnabled;
 void EnablePixelShaders(); //00988110
 void DisablePixelShaders(); //00988190
 void EnableVertexShaders(); //00987fe0
 void DisableVertexShaders(); //009880e0
};
struct CEngineLightingManager_Methods:CEngineLightingManager {void DisablePointLights();}; //00b44d50
extern CRenderStateManager* g_pRenderStateManager_01436e18; //01436e18
extern CShaderRenderManager* g_pShaderRenderManager; //01436e14
extern CEngineLightingManager_Methods* g_pLightingManager; //01436e9c
static __forceinline void ClearNormalizeNormals()
{
 CRenderStateManager* manager=g_pRenderStateManager_01436e18;
 CStateInfo& state=reinterpret_cast<CDeviceStateNormalizeView*>(manager->DeviceState)->NormalizeNormals;
 if(state.DesiredState!=0){
  unsigned long oldMask=state.BookmarkMask;
  unsigned long activeMask=manager->CurrentBookmarkMask;
  if((oldMask & activeMask)==0){
   state.BookmarkMask=activeMask|oldMask;
   reinterpret_cast<CStateStackElement*>(manager->StateStack)[manager->StateStackSize].State=&state;
   reinterpret_cast<CStateStackElement*>(manager->StateStack)[manager->StateStackSize].Value=state.DesiredState;
   ++manager->StateStackSize;
  }
  state.DesiredState=0;
  if(!state.DirtyListFlag){state.DirtyListFlag=true;manager->StateUpdateList[manager->StateUpdateListSize]=&state;++manager->StateUpdateListSize;}
 }
}
struct CEnginePrimitiveRenderer_Methods:CEnginePrimitiveRenderer {void ResetWorldTransform();void PostRenderLayer();};
void CEnginePrimitiveRenderer_Methods::PostRenderLayer()
{
 reinterpret_cast<long&>(_pad_0x78)=-1;
 ClearNormalizeNormals();
 g_pLightingManager->DisablePointLights();
 ResetWorldTransform();
 if(_pad_0x260[0]!=g_pShaderRenderManager->PixelShadersEnabled){if(_pad_0x260[0])g_pShaderRenderManager->EnablePixelShaders();else g_pShaderRenderManager->DisablePixelShaders();}
 if(_pad_0x260[1]!=g_pShaderRenderManager->VertexShadersEnabled){if(_pad_0x260[1])g_pShaderRenderManager->EnableVertexShaders();else g_pShaderRenderManager->DisableVertexShaders();}
}