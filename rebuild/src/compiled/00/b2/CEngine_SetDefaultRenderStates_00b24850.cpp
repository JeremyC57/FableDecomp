// CEngine::SetDefaultRenderStates() -- put the engine's render manager into its default state: per texture stage (8) alpha op off and
//   texture/diffuse arguments, the default alpha test / shading / fill / cull / z-buffer / fog / alpha-ref / specular / dither states,
//   per stage the anisotropy, LOD bias, linear mip and filter modes and clamped UVW, the blend modes, stage 0 modulate and stage 1 off,
//   texturing / blending / clipping / lighting, the default fog states, the vertex shader constant layout 3, no texture transforms, the
//   UV-compression constants (1/2048, -8) and, when EnableWireFrame is set, wireframe fill
// Inline-budget: header-contract inline setters only (FABLE_FORCEINLINE SetRenderState / AddStateToUpdateList / Set* of
//   engine/CRenderStateManager.h, SetTextureEnable of engine/CRenderManagerCore.h; the default header branches, no setter opt-in) and
//   FABLE_RETAIL_ASSERT_DEAD_ARM (fable_assert.h, emits no bytes); no flag and no pragma. The FABLE_RETAIL_CENGINE / _CENGINELIGHTINGMANAGER
//   / _CDISPLAYMANAGER / _SCOPED_PTR defines select the header class layouts (without any of them the TU does not compile). The draft's
//   CRENDERSTATEMANAGER_EARLY_RETURN and _DEFAULT_TRANSFORM opt-ins change no byte and are dropped (README ablation).
// Leaf: ?SetDefaultRenderStates@CEngine@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B24850, 4344 bytes, private thiscall, plain `ret`, no EH frame, `sub esp,8`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetDefaultRenderStates@CEngine@@AAEXXZ (FableWin 0x02C9B640, Debug\Engine.obj, 847 debug bytes =
//   Ego_d.pdb's length).
// Callees (out of line): CEngineLightingManager::SetupDefaultFogStates 0x00B47630 (GPLightingManager 0x01436E9C),
//   CShaderRenderManager::SetVertexShaderConstantLayout 0x0098B5E0 / SetTextureTransformCount 0x00988630 /
//   SetUVCompressionVertexShaderConstant 0x00988730 (GPShaderRenderManager 0x01436E14), CRenderManagerCore::EnableXBoxShaderConstants
//   0x00A0AA20 (GPRenderManager 0x01436E18), CDisplayManager::GetVertexShaderVersion 0x009BEDE0 (GPDisplayManager 0x01436E1C).
//   Data: the CRenderStateManager D3D*Table arrays (0x01396E70 ...).
// Source shape: Ego_d.pdb's locals and line table (lines 603..674): function scope `const int MAX_TEXTURE_STAGES = 8` (type 0x1C54 = const
//   int), `float uv_scale`, `float uv_bias` (frame order uv_scale, uv_bias, MAX_TEXTURE_STAGES: declared at the top, assigned at lines
//   666 / 667 just before the call at 668), and a `long n` in each loop's block. SetTextureTransformMode in the second loop takes the
//   constant stage 0, as retail's bytes do (Ego_d line 640 is 15 bytes, a constant push, where the n-indexed setters are 17).
// Inferred (two FITTED devices; coordinator ruling 2026-11-20 under the owner's delegation): retail's 38 inline SetRenderState expansions
//   differ from the Ego_d text only in two c2 tie orders. (a) The operand order of the bookmark `test` / `or`: retail switches to
//   `test esi,eax` at expansion 27 (0x00B2543B), the Ego_d text at 28. (b) The load order of the two masks: retail loads BookmarkMask
//   before CurrentBookmarkMask from expansion 31 (0x00B255F9) on, the Ego_d text never does. Measured on this text: (a) moves only with
//   one more dead function-level named scalar (any type, initialiser or count >= 1); (b) moves only with the number of dead
//   FABLE_RETAIL_ASSERT arms (0..4: never, 5..13: from 31, 14+: from 7). Neither device emits a byte.
//   (1) `version` (below): retail calls GetVertexShaderVersion 0x009BEDE0 at 0x00B258A7 and discards the result; without the local,
//   DIFFER(4344v4344) (ablation ab_noversion). The bytes prove one more function-level named scalar, not its name, type or initialiser.
//   Ego_d lists none (its line 664 is 34 bytes, neither a 14-byte store nor an 18-byte call + compare + branch; unidentified). The name
//   is descriptive.
//   (2) five release-mode asserts: the count is fitted (the bottom of the 5..13 window); the conditions are unconstrained by the bytes
//   (the four manager pointers this function dereferences, and vs_1_1 = 1100, the threshold the landed 0x00B3B030 / 0x00B33B50 test).
//   Ego_d / FableWin (both 847 bytes) have no assert-sized line here, so their debug revision of this function carries no asserts.
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/Direct3D9Types.h"
#include "engine/CEngine.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CDisplayManager.h"
namespace NEngineInternal {
extern CDisplayManager*        GPDisplayManager;         // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A
extern CEngineLightingManager* GPLightingManager;        // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A
extern CRenderManager*         GPRenderManager;          // 0x01436E18 ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A
extern CShaderRenderManager*   GPShaderRenderManager;    // 0x01436E14 ?GPShaderRenderManager@NEngineInternal@@3PAVCShaderRenderManager@@A
}
using namespace NEngineInternal;

void CEngine::SetDefaultRenderStates()
{
    // Inferred: release-mode asserts, FITTED count (5 of the 5..13 window), conditions unconstrained -- see the header comment.
    FABLE_RETAIL_ASSERT(NEngineInternal::GPRenderManager != 0);
    FABLE_RETAIL_ASSERT(NEngineInternal::GPShaderRenderManager != 0);
    FABLE_RETAIL_ASSERT(NEngineInternal::GPLightingManager != 0);
    FABLE_RETAIL_ASSERT(NEngineInternal::GPDisplayManager != 0);
    float uv_scale;
    float uv_bias;
    const int MAX_TEXTURE_STAGES = 8;
    for (long n = 0; n < MAX_TEXTURE_STAGES; ++n) {
        GPRenderManager->SetTextureAlphaOperation(n, TEXTURE_OP_DISABLE);
        GPRenderManager->SetTextureArgument1(n, TEXTURE_ARG_TEXTURE);
        GPRenderManager->SetTextureArgument2(n, TEXTURE_ARG_DIFFUSE);
        GPRenderManager->SetTextureAlphaArgument1(n, TEXTURE_ARG_TEXTURE);
        GPRenderManager->SetTextureAlphaArgument2(n, TEXTURE_ARG_DIFFUSE);
    }
    GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_ALWAYS);
    GPRenderManager->SetPolyShadeMode(POLY_SHADE_GOURAUD);
    GPRenderManager->SetPolyFillMode(POLY_FILL_SOLID);
    GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    GPRenderManager->SetZBufferWriteEnable(true);
    GPRenderManager->SetZBufferTestFunc(ZBUFFER_FUNC_LESS_EQUAL);
    GPRenderManager->SetZBufferEnable(true);
    GPRenderManager->SetFogEnable(false);
    GPRenderManager->SetFogColour(0xffffff);
    GPRenderManager->SetFogStart(0.0f);
    GPRenderManager->SetFogEnd(1.0f);
    GPRenderManager->SetAlphaRef(0x80);
    GPRenderManager->SetSpecularEnable(false);
    GPRenderManager->SetDitherEnable(true);
    for (long n = 0; n < MAX_TEXTURE_STAGES; ++n) {
        GPRenderManager->SetTextureMaxAnisotropy(n, AnisotropicFilteringLevel);
        GPRenderManager->SetTextureMipmapLODBias(n, 0.0f);
        GPRenderManager->SetTextureMipMapMode(n, TEXTURE_MIPMAP_LINEAR);
        GPRenderManager->SetTextureFilterModeMin(n, TEXTURE_FILTER_LINEAR);
        GPRenderManager->SetTextureFilterModeMag(n, TEXTURE_FILTER_LINEAR);
        GPRenderManager->SetTextureUVModeU(n, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureUVModeV(n, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureUVModeW(n, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureTransformMode(0, TEXTURE_TRANSFORM_DIABLED);
    }
    GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ZERO);
    GPRenderManager->SetTextureOperation(1, TEXTURE_OP_DISABLE);
    GPRenderManager->SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
    GPRenderManager->SetTextureOperation(0, TEXTURE_OP_MODULATE);
    GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
    GPRenderManager->SetTextureEnable(EnableTextures);
    GPRenderManager->SetAlphaBlendEnable(false);
    GPRenderManager->SetClippingEnable(true);
    GPRenderManager->SetLightingEnable(false);
    NEngineInternal::GPLightingManager->SetupDefaultFogStates();
    GPShaderRenderManager->SetVertexShaderConstantLayout(3);
    GPShaderRenderManager->SetTextureTransformCount(0);
    GPRenderManager->EnableXBoxShaderConstants(true);
    // Inferred: retail-only dead local, not in Ego_d, descriptive name -- the discarded 0x009BEDE0 result; DIFFER(4344v4344) without it.
    unsigned long version = NEngineInternal::GPDisplayManager->GetVertexShaderVersion();
    FABLE_RETAIL_ASSERT(version >= 1100);
    uv_scale = 1.0f / 2048.0f;
    uv_bias = -8.0f;
    GPShaderRenderManager->SetUVCompressionVertexShaderConstant(uv_scale, uv_scale, uv_bias, uv_bias);
    if (EnableWireFrame)
        GPRenderManager->SetPolyFillMode(POLY_FILL_WIRE);
}