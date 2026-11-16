// CEnginePrimitiveRenderer::PostRenderLayer -- reset current layer, normalize-normals tracked state, lights/world transform, then saved shader enable flags. Retail00b32230 complete207bytes+1int3. Shared renderer/state/lighting headers; donor leaves renderer saved layer+78 and shaderflags+260/+261 opaque. DeviceState NormalizeNormals at+230 (manager2a48), nativeCStateInfo20bytes and statecapture8bytes. CalleesB44D50/B32100/988110/988190/987FE0/9880E0.
#include "fable_compiler.h"
// NF45 (round 7): the render-state manager is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderStateManager at offset 0), and the inline
//   NormalizeNormals clear (DeviceState +0x230, core +0x2A48) is the shared CRenderStateManager::SetNormalizeNormals(false) (overlay
//   inline opt-in). Was g_pRenderStateManager_01436e18 with local CStateInfo / CStateStackElement / CDeviceStateNormalizeView models,
//   a ClearNormalizeNormals helper and a non-MSVC fallback branch.
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETNORMALIZENORMALS_INLINE
#include "engine/CRenderManager.h"
#include "engine/CEnginePrimitiveRenderer.h"
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#include "engine/CEngineLightingManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
class CShaderRenderManager { public:unsigned char EarlierMembers[0x390];bool VertexShadersEnabled,PixelShadersEnabled;
 void EnablePixelShaders(); //00988110
 void DisablePixelShaders(); //00988190
 void EnableVertexShaders(); //00987fe0
 void DisableVertexShaders(); //009880e0
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; } //01436e14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; } //01436e9c (CEngineLightingManager::DisablePointLights 00b44d50)
struct CEnginePrimitiveRenderer_Methods:CEnginePrimitiveRenderer {void ResetWorldTransform();void PostRenderLayer();};
void CEnginePrimitiveRenderer_Methods::PostRenderLayer()
{
 reinterpret_cast<long&>(_pad_0x78)=-1;
 NEngineInternal::GPRenderManager->SetNormalizeNormals(false);
 NEngineInternal::GPLightingManager->DisablePointLights();
 ResetWorldTransform();
 if(_pad_0x260[0]!=NEngineInternal::GPShaderRenderManager->PixelShadersEnabled){if(_pad_0x260[0])NEngineInternal::GPShaderRenderManager->EnablePixelShaders();else NEngineInternal::GPShaderRenderManager->DisablePixelShaders();}
 if(_pad_0x260[1]!=NEngineInternal::GPShaderRenderManager->VertexShadersEnabled){if(_pad_0x260[1])NEngineInternal::GPShaderRenderManager->EnableVertexShaders();else NEngineInternal::GPShaderRenderManager->DisableVertexShaders();}
}