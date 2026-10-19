// CEnginePrimitiveRenderer::PostRenderLayer -- reset current layer, normalize-normals tracked state, lights/world transform, then saved shader enable flags. Retail00b32230 complete207bytes+1int3. Shared renderer/state/lighting headers; donor leaves renderer saved layer+78 and shaderflags+260/+261 opaque. DeviceState NormalizeNormals at+230 (manager2a48), nativeCStateInfo20bytes and statecapture8bytes. CalleesB44D50/B32100/988110/988190/987FE0/9880E0.
#ifndef _MSC_VER
#define __forceinline inline
struct CEnginePrimitiveRenderer {unsigned char _pad_0x78[sizeof(long)];unsigned char _pad_0x260[4];};
struct CEngineLightingManager {};
struct CStateInfo;
struct CRenderStateManager {unsigned char StateStack[32768];CStateInfo* StateUpdateList[256];long StateStackSize,StateUpdateListSize;unsigned long CurrentBookmarkMask;unsigned char DeviceState[16384];};
#else
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CRenderStateManager.h"
#include "engine/CEngineLightingManager.h"
#endif
struct CStateInfo {unsigned long CurrentState,DesiredState,BookmarkMask,DeviceStateID;bool DirtyListFlag;unsigned char StateType,TextureStage;};
struct CStateStackElement {CStateInfo* State;unsigned long Value;};
struct CDeviceStateNormalizeView {unsigned char EarlierStates[0x230];CStateInfo NormalizeNormals;};
struct CShaderRenderManager {unsigned char EarlierMembers[0x390];bool VertexShadersEnabled,PixelShadersEnabled;
 void EnablePixelShaders(); //00988110
 void DisablePixelShaders(); //00988190
 void EnableVertexShaders(); //00987fe0
 void DisableVertexShaders(); //009880e0
};
struct CEngineLightingManager_Methods:CEngineLightingManager {void DisablePointLights();}; //00b44d50
extern CRenderStateManager* g_pRenderStateManager; //01436e18
extern CShaderRenderManager* g_pShaderRenderManager; //01436e14
extern CEngineLightingManager_Methods* g_pLightingManager; //01436e9c
static __forceinline void ClearNormalizeNormals()
{
 CRenderStateManager* manager=g_pRenderStateManager;
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

#include <cstdio>
#include <cstring>
static int failures=0,step=0,pixelCalls=0,vertexCalls=0;static bool wantPixel,wantVertex;
static CEnginePrimitiveRenderer_Methods* active=0;static CStateInfo* tracked=0;static CShaderRenderManager* swapManager=0;
static void check(bool b){if(!b)++failures;}
CRenderStateManager* g_pRenderStateManager=0;CShaderRenderManager* g_pShaderRenderManager=0;CEngineLightingManager_Methods* g_pLightingManager=0;
void CEngineLightingManager_Methods::DisablePointLights(){check(step==0 && reinterpret_cast<long&>(active->_pad_0x78)==-1 && tracked->DesiredState==0);step=1;}
void CEnginePrimitiveRenderer_Methods::ResetWorldTransform(){check(this==active && step==1);step=2;}
void CShaderRenderManager::EnablePixelShaders(){check(step==2 && wantPixel && !PixelShadersEnabled);++pixelCalls;PixelShadersEnabled=true;if(swapManager)g_pShaderRenderManager=swapManager;}
void CShaderRenderManager::DisablePixelShaders(){check(step==2 && !wantPixel && PixelShadersEnabled);++pixelCalls;PixelShadersEnabled=false;if(swapManager)g_pShaderRenderManager=swapManager;}
void CShaderRenderManager::EnableVertexShaders(){check(step==2 && wantVertex && !VertexShadersEnabled);++vertexCalls;VertexShadersEnabled=true;}
void CShaderRenderManager::DisableVertexShaders(){check(step==2 && !wantVertex && VertexShadersEnabled);++vertexCalls;VertexShadersEnabled=false;}
static void run(unsigned long desired,unsigned long mask,unsigned long activeMask,bool dirty,bool oldP,bool oldV,bool savedP,bool savedV,bool swap){
 CRenderStateManager manager;std::memset(&manager,0,sizeof(manager));CEnginePrimitiveRenderer_Methods renderer;CShaderRenderManager shader,other;CEngineLightingManager_Methods lights;
 CStateInfo* state=&reinterpret_cast<CDeviceStateNormalizeView*>(manager.DeviceState)->NormalizeNormals;state->CurrentState=83;state->DesiredState=desired;state->BookmarkMask=mask;state->DeviceStateID=91;state->DirtyListFlag=dirty;state->StateType=2;state->TextureStage=4;
 manager.CurrentBookmarkMask=activeMask;manager.StateStackSize=2;manager.StateUpdateListSize=3;CStateStackElement* stack=reinterpret_cast<CStateStackElement*>(manager.StateStack);stack[1].State=state;stack[1].Value=731;
 renderer._pad_0x260[0]=savedP;renderer._pad_0x260[1]=savedV;reinterpret_cast<long&>(renderer._pad_0x78)=89;shader.PixelShadersEnabled=oldP;shader.VertexShadersEnabled=oldV;other.PixelShadersEnabled=savedP;other.VertexShadersEnabled=!savedV;
 g_pRenderStateManager=&manager;g_pShaderRenderManager=&shader;g_pLightingManager=&lights;active=&renderer;tracked=state;step=pixelCalls=vertexCalls=0;wantPixel=savedP;wantVertex=savedV;swapManager=swap?&other:0;
 renderer.PostRenderLayer();bool capture=desired && !(mask&activeMask);bool queue=desired && !dirty;
 check(step==2 && reinterpret_cast<long&>(renderer._pad_0x78)==-1 && state->DesiredState==0 && state->CurrentState==83 && state->DeviceStateID==91 && state->StateType==2 && state->TextureStage==4);
 check(state->BookmarkMask==(capture?(mask|activeMask):mask) && state->DirtyListFlag==(dirty||desired!=0));
 check(manager.StateStackSize==2+(capture?1:0) && manager.StateUpdateListSize==3+(queue?1:0) && stack[1].Value==731 && stack[1].State==state);
 if(capture)check(stack[2].State==state && stack[2].Value==desired);if(queue)check(manager.StateUpdateList[3]==state);
 bool swapped=swap && savedP!=oldP;check(pixelCalls==(savedP!=oldP?1:0));check(vertexCalls==(swapped?1:(savedV!=oldV?1:0)));check(g_pShaderRenderManager->VertexShadersEnabled==savedV);
}
int main(){
#ifdef _MSC_VER
 check(sizeof(CStateInfo)==20 && sizeof(CStateStackElement)==8);
#endif
 for(int p=0;p<2;++p)for(int v=0;v<2;++v)for(int sp=0;sp<2;++sp)for(int sv=0;sv<2;++sv)run(9,2,4,false,p!=0,v!=0,sp!=0,sv!=0,false);
 run(0,2,4,false,false,true,true,false,false);run(9,6,4,true,true,false,false,true,false);run(9,2,0,true,false,false,true,true,false);run(9,1,2,false,false,true,true,true,true);
 if(failures){std::printf("00b32230_TEST FAIL %d\n",failures);return 1;}std::printf("00b32230_TEST PASS\n");return 0;}