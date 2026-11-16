// 00bf0b20 CEnginePrimitiveRenderUtil::SetupOutlineFillStates
// Leaf: ?SetupOutlineFillStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z
// Retail: 0x00BF0B20,1290 bytes; public thiscall.
// Flags: none
// Evidence: both PDBs; original module922 parameter/local names; native state sequence.
// Debug donor has different state values and omits colour upload; native release controls those operations.
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

void CEnginePrimitiveRenderUtil::SetupOutlineFillStates(const CEngineMeshEffectOutline& effect, float mesh_scale)
{
    float width_world_space=effect.WidthWorldSpace*NEngineInternal::GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    float width_screen_space=effect.WidthScreenSpace*NEngineInternal::GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    CRGBColour colour=effect.Colour;
    // The mesh-scale reciprocal and the percent factor multiply inside the upload arguments (no `width *= 1.0f/mesh_scale;` statements):
    // an assignment to a float local leaves a no-op float-store tuple in c2's block DAG, and one more of those on the fmul chain
    // raises the G store's scheduler height to a tie with the B load (retail 0x00BF0B36 `mov cl,[ebp+0x14]` is issued before the
    // G store 0x00BF0B39 `mov [esp+0x1d],al`).
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,width_world_space*(1.0f/mesh_scale)*0.01f,width_screen_space*(1.0f/mesh_scale)*0.01f,0.0f,0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, CRGBFloatColour(colour));
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetStencilEnable(true);
    NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_EQUAL);
    NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_ZERO);
    unsigned long stencil_mask = 1 << (effect.StencilBit + 2);
    NEngineInternal::GPRenderManager->SetStencilMask(stencil_mask);
    NEngineInternal::GPRenderManager->SetStencilRef(stencil_mask);
}