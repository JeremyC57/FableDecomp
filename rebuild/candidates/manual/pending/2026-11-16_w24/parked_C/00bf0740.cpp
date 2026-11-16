// CEnginePrimitiveRenderUtil::SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float mesh_scale) -- upload the outline widths (world and screen, scaled by the outline-glow scale factor / mesh_scale * 0.01) to vertex constant 0 and white to constant 1, then set the outline stencil pass states
// Leaf: ?SetupOutlineStencilStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF0740, 992 bytes, thiscall `ret 8` (`this` unused).
// Flags: none
// Name: Ego_r.pdb / FableWin.pdb ?SetupOutlineStencilStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z. The two
//   (effect, float) outline setters are told apart by pass: the renderers call 0x00BF0740 for RENDER_PASS_OUTLINE_STENCIL (12) and
//   0x00BF0B20 for RENDER_PASS_OUTLINE_FILL (13) (0x00BA2350, 0x00BB4615), and this body writes the stencil (colour writes off,
//   STENCIL_FUNC_ALWAYS / STENCIL_OP_MODE_REPLACE, write mask and reference = the outline's stencil bit) and uploads white, where
//   0x00BF0B20 tests the stencil and uploads the outline colour. The landed row 0x00BF0B20 carries this leaf; it is
//   SetupOutlineFillStates (NAME_FIXES.tsv; already queued as land1113fy / land1115ga).
// Shape: after the landed sibling 0x00BF0B20 (shared headers, the mesh-scale reciprocal folded into the upload arguments) and
//   0x00BEFD90 SetupOutlineSmoothStencilStates (same state list without the fog state and the extra extrusion).
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(ulong, float, float, float, float) 0x00989B00 and (ulong, const
//   CRGBFloatColour&) 0x00989B50; the CRenderStateManager setters are expanded (no retail copies).
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_PASS22_RENDERUTIL_COMPLETE_ORIGINAL_DECLARATION
#define FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_ORIGINAL_COMPLETE_PASS22
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_ORIGINAL_FORMALS_PASS22
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINEMESHEFFECT_ORIGINAL_DECLARATIONS_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCILSTATES
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEnginePrimitiveRenderUtil.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                          // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }              // 0x01436E14
namespace NEngineInternal { extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow; } // 0x01436E3C

void CEnginePrimitiveRenderUtil::SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float mesh_scale)
{
    float width_world_space=effect.WidthWorldSpace*NEngineInternal::GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    float width_screen_space=effect.WidthScreenSpace*NEngineInternal::GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,width_world_space*(1.0f/mesh_scale)*0.01f,width_screen_space*(1.0f/mesh_scale)*0.01f,0.0f,0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, CRGBFloatColour(1,1,1,1));
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetStencilEnable(true);
    NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_ALWAYS);
    NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
    unsigned long stencil_mask = 1 << (effect.StencilBit + 2);
    NEngineInternal::GPRenderManager->SetStencilWriteMask(stencil_mask);
    NEngineInternal::GPRenderManager->SetStencilRef(stencil_mask);
}
