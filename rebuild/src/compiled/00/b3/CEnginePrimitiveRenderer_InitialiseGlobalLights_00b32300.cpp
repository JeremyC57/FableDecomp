// CEnginePrimitiveRenderer::InitialiseGlobalLights() -- update the lighting manager's global lighting states, then set the two lighting tracked render states from the default @ 0x01396FF4, hand the render core two white colours and enable the tracked state +0x2a48
// Leaf: InitialiseGlobalLights
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32300, length 416 bytes (body 0x192 + int3 padding), plain `ret` (`this` unused).
// Callees: CEngineLightingManager::UpdateGlobalLightingStates @ 0x00B46C80 (global @ 0x01436E9C); a render-core method taking two
//   `const CRGBColour&` @ 0x00A0AA10 (an empty `ret 8` stub in the PC build; catalogued `$E2`).
// Globals: the render manager core @ 0x01436E18 with its tracked render-state block (capture stack +0x08, dirty list +0x2008,
//   counts +0x2808/+0x280C, capture mask +0x2814, 0x14-byte states; the model of the landed EnableFog 0x00B46890); the states
//   touched are +0x2a70, +0x2a5c (value: the global @ 0x01396FF4) and +0x2a48 (value 1).
// Shape: the manager-side `__forceinline SetState(state, value)` of EnableFog; the colours are CRGBColour temporaries (B,G,R,A
//   bytes, ctor stores R,G,B,A -> +2,+1,+0,+3), the second argument's built first.
#include "fable_compiler.h"
#include "engine/CEnginePrimitiveRenderer.h"

// NF45 (round 7): the render manager is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A; engine/CRenderManager.h, CRenderManagerCore / CRenderStateManager at offset 0).
//   The local CRenderManagerCoreView / CTrackedRenderState model and g_renderStateDefault_01396ff4 are replaced by the shared
//   CRenderStateManager setters: +0x2A70 DiffuseMaterialColourSource, +0x2A5C AmbientMaterialColourSource (D3DMaterialColourSourceTable
//   0x01396FF4 [MATERIAL_COLOUR_SOURCE_MATERIAL]) and +0x2A48 NormalizeNormals (overlay inline opt-ins); the stub 0x00A0AA10 is
//   CRenderManagerCore::SetMaterial(const CRGBColour&, const CRGBColour&) (overlay opt-in; was SetGlobalLightColours); the colours are
//   the shared engine/CRGBColour.h.
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETNORMALIZENORMALS_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETAMBIENTMATERIALCOLOURSOURCE_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETDIFFUSEMATERIALCOLOURSOURCE_INLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE_SETMATERIAL
#include "engine/CRenderManager.h"
#include "engine/CRGBColour.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

class CEngineLightingManager { public:
    void UpdateGlobalLightingStates();             // 0x00B46C80
};
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A

struct CEnginePrimitiveRenderer_Methods : CEnginePrimitiveRenderer {
    void InitialiseGlobalLights();
};

void CEnginePrimitiveRenderer_Methods::InitialiseGlobalLights()
{
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    NEngineInternal::GPRenderManager->SetDiffuseMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);
    NEngineInternal::GPRenderManager->SetAmbientMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);
    NEngineInternal::GPRenderManager->SetMaterial(CRGBColour(255, 255, 255, 255), CRGBColour(255, 255, 255, 255));
    NEngineInternal::GPRenderManager->SetNormalizeNormals(true);
}