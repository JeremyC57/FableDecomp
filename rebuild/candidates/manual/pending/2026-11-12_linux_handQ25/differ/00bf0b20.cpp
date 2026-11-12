// CEnginePrimitiveRenderUtil::SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float scale)
// Grade: DIFFER(1290v1290) -- prologue scheduling only (2 instructions): retail stores the first colour byte (G, `mov [esp+1Dh],al`) and loads the outline-glow pointer before `push esi`, then loads A (`mov dl,[ebp+17h]`); ours loads A before `push esi` and stores G after it (same slot, arg +1). Everything after 0x00BF0B46 matches. Unchanged across 14 grades (colour copy flavours: memberwise / Initialise / 4-arg / byte locals; scope block; named width / glow / inv-scale locals; parameter reuse).
// Uploads the outline widths (world and screen width times the outline-glow scale factor, 1/scale and 0.01) as user vertex constant 0 and the outline
// colour as constant 1, then set the outline pass states: no z writes, no alpha test, alpha blending on (source / dest blend from
// D3DAlphaBlendModeTable), stencil on, stencil func and pass op from their tables, and the stencil mask and reference both
// 1 << (StencilBit + 2).
// Leaf: ?SetupOutlineStencilStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF0B20, length 1296 bytes, `ret 8` (`this` unused).
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(ulong, float x4) 0x00989B00 and (ulong, const CRGBFloatColour&)
//   0x00989B50 on NEngineInternal::GPShaderRenderManager (0x01436E14); the inline CRenderStateManager setters on
//   NEngineInternal::GPRenderManager (0x01436E18): ZBufferWriteEnable +0x2854, AlphaTestEnable +0x28CC, AlphaBlendEnable +0x28B8,
//   SourceAlphaBlendMode +0x291C / DestAlphaBlendMode +0x2930 (D3DAlphaBlendModeTable 0x01396F68 entries 0x01396F78 / 0x01396F7C),
//   StencilEnable +0x2B88, StencilFunc +0x2BD8 (D3DStencilFuncModeTable entry 0x01396E20), StencilOpPass +0x2BC4
//   (D3DStencilOperationModeTable entry 0x01396F3C), StencilMask +0x2C00, StencilRef +0x2BEC.
// Globals: NEngineInternal::GPScreenEffectOutlineGlow 0x01436E3C (the WellingtonGame PDB's
//   ?GPScreenEffectOutlineGlow@NEngineInternal@@3PAVCEngineScreenEffectOutlineGlow@@A; CEngine::Initialise 0x00B26360 stores the
//   0x1E4-byte CEngineScreenEffectOutlineGlow there), its OutlineGlowScaleFactor +0x1BC (Ego_r.pdb).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINEMESHEFFECT
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

void CEnginePrimitiveRenderUtil::SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float scale)
{
    CRGBColour colour = effect.Colour;
    float inv_scale = 1.0f / scale;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,
        effect.WidthWorldSpace * NEngineInternal::GPScreenEffectOutlineGlow->OutlineGlowScaleFactor * inv_scale * 0.01f,
        effect.WidthScreenSpace * NEngineInternal::GPScreenEffectOutlineGlow->OutlineGlowScaleFactor * inv_scale * 0.01f,
        0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, CRGBFloatColour(colour));
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetStencilEnable(true);
    NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_EQUAL);
    NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_ZERO);
    unsigned long mask = 1 << (effect.StencilBit + 2);
    NEngineInternal::GPRenderManager->SetStencilMask(mask);
    NEngineInternal::GPRenderManager->SetStencilRef(mask);
}
