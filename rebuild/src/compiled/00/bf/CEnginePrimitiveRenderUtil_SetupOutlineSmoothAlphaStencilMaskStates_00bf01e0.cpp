// CEnginePrimitiveRenderUtil::SetupOutlineSmoothAlphaStencilMaskStates(const CEngineMeshEffectOutline& effect) -- zero user vertex constant 0 and set constant 1 to white, then set the outline's smooth-alpha stencil-mask pass states: no z writes, no colour writes, alpha test on (greater than the effect's TextureAlphaRef), no alpha blending, no fog, stencil always passing and replacing the effect's stencil bit (write mask 1 << (StencilBit + 2)) with reference 0
// Leaf: ?SetupOutlineSmoothAlphaStencilMaskStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF01E0, length 1376 bytes, `ret 4` (`this` unused).
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(ulong, float x4) 0x00989B00 and (ulong, const CRGBFloatColour&)
//   0x00989B50 on NEngineInternal::GPShaderRenderManager (0x01436E14); the inline CRenderStateManager setters on
//   NEngineInternal::GPRenderManager (0x01436E18): ZBufferWriteEnable +0x2854, ColourWriteEnable +0x2840, AlphaTestEnable +0x28CC,
//   AlphaBlendEnable +0x28B8, AlphaRef +0x28E0, AlphaTestFunc +0x28A4 <- D3DAlphaFuncModeTable[GREATER] 0x01396DE8, FogEnable
//   +0x2944, StencilEnable +0x2B88, StencilFunc +0x2BD8 <- D3DStencilFuncModeTable[ALWAYS] 0x01396E34, StencilOpPass +0x2BC4 <-
//   D3DStencilOperationModeTable[REPLACE] 0x01396F40, StencilWriteMask +0x2C14, StencilRef +0x2BEC.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEnginePrimitiveRenderUtil.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14

void CEnginePrimitiveRenderUtil::SetupOutlineSmoothAlphaStencilMaskStates(const CEngineMeshEffectOutline& effect)
{
    // The alpha reference is read before the constant uploads (retail 0x00BF01F1 `movzx edi,byte ptr [ebp+19h]` ahead of the
    // first call; inline at its setter it is read after them and edi holds 0 for the pushes).
    unsigned long alphaRef = effect.TextureAlphaRef;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaRef(alphaRef);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPRenderManager->SetStencilEnable(true);
    NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_ALWAYS);
    NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
    NEngineInternal::GPRenderManager->SetStencilWriteMask(1 << (effect.StencilBit + 2));
    NEngineInternal::GPRenderManager->SetStencilRef(0);
}