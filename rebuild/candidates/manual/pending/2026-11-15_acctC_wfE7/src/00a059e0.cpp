// CRenderStateManager::SetDefaultRenderStates() -- request the engine's default value of every render, texture-stage and sampler state (all eight stages reset, stage 0 modulating texture by diffuse), each through the bookmarked SetRenderState queue
// Leaf: ?SetDefaultRenderStates@CRenderStateManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A059E0, 7958 bytes, thiscall `ret`, frame 0x18, no calls (78 inline SetRenderState expansions).
// Setters the header only declares are defined here FABLE_FORCEINLINE, as the landed sibling 0x00BA1B00 DisplayMirroredTextures does for
//   SetTextureFactor (retail inlines them in every TU; this TU contributed the unreferenced COMDAT copies 0x00A08D10..0x00A09D10).
// FITTED (chosen by grading; code-identical alternatives, the c2 record-id tie of lessons wfB4 on the bookmark test / or operand order):
//   (1) the first texture-factor write calls the private SetRenderState directly instead of SetTextureFactor: the SetTextureFactor
//       inline parameter record pushes block 14's key out of the first free id run (retail 0x00A05F72 loads CurrentBookmarkMask first);
//   (2) SetPointSizeMin / SetPointSizeMax read the float's bits into a named local: two records at expansion time that move the
//       SetStencilFunc expansion (retail 0x00A0764A loads the state's mask first) past the second free id run. FableWin.pdb's debug
//       copies of these two (98i/358b) also carry a float ASSERT each (dead in release, no effect on the ids: measured).
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_POINTSPRITE_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE
#include "engine/CRenderStateManager.h"

// Setters CRenderStateManager.h only declares: inline in retail (expanded at every call site here and in other TUs).

FABLE_FORCEINLINE void CRenderStateManager::SetVertexBlending(VERTEX_BLEND_MODE mode) { SetRenderState(D3DVertexBlendModeTable[mode], DeviceState.VertexBlendMode); }
FABLE_FORCEINLINE void CRenderStateManager::SetFogDensity(float density) { SetRenderState(*(unsigned long*)&density, DeviceState.FogDensity); }
FABLE_FORCEINLINE void CRenderStateManager::SetFogMode(FOG_MODE mode) { SetRenderState(D3DFogTableMode[mode], DeviceState.FogMode); }
FABLE_FORCEINLINE void CRenderStateManager::SetFogVertexMode(FOG_MODE mode) { SetRenderState(D3DFogTableMode[mode], DeviceState.FogVertexMode); }
FABLE_FORCEINLINE void CRenderStateManager::SetTextureFactor(unsigned long factor) { SetRenderState(factor, DeviceState.TextureFactor); }
FABLE_FORCEINLINE void CRenderStateManager::SetNormalizeNormals(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.NormalizeNormals); }
FABLE_FORCEINLINE void CRenderStateManager::SetAmbientMaterialColourSource(MATERIAL_COLOUR_SOURCE source) { SetRenderState(D3DMaterialColourSourceTable[source], DeviceState.AmbientMaterialColourSource); }
FABLE_FORCEINLINE void CRenderStateManager::SetDiffuseMaterialColourSource(MATERIAL_COLOUR_SOURCE source) { SetRenderState(D3DMaterialColourSourceTable[source], DeviceState.DiffuseMaterialColourSource); }
FABLE_FORCEINLINE void CRenderStateManager::SetSpecularMaterialColourSource(MATERIAL_COLOUR_SOURCE source) { SetRenderState(D3DMaterialColourSourceTable[source], DeviceState.SpecularMaterialColourSource); }
FABLE_FORCEINLINE void CRenderStateManager::SetPointSize(float size) { SetRenderState(*(unsigned long*)&size, DeviceState.PointSize); }
FABLE_FORCEINLINE void CRenderStateManager::SetPointSizeMin(float size) { const unsigned long value = *(unsigned long*)&size; SetRenderState(value, DeviceState.PointSizeMin); }   // FITTED (2)
FABLE_FORCEINLINE void CRenderStateManager::SetPointSizeMax(float size) { const unsigned long value = *(unsigned long*)&size; SetRenderState(value, DeviceState.PointSizeMax); }   // FITTED (2)
FABLE_FORCEINLINE void CRenderStateManager::SetPointScaleConstantFactor(float factor) { SetRenderState(*(unsigned long*)&factor, DeviceState.PointScaleConstantFactor); }
FABLE_FORCEINLINE void CRenderStateManager::SetPointScaleDistanceFactor(float factor) { SetRenderState(*(unsigned long*)&factor, DeviceState.PointScaleDistanceFactor); }
FABLE_FORCEINLINE void CRenderStateManager::SetPointScaleDistanceSqrFactor(float factor) { SetRenderState(*(unsigned long*)&factor, DeviceState.PointScaleDistanceSqrFactor); }
FABLE_FORCEINLINE void CRenderStateManager::SetTextureCoordWrapUVW(long stage, bool u, bool v, bool w) { SetRenderState((u ? 1 : 0) | (v ? 2 : 0) | (w ? 4 : 0), DeviceState.TextureCoordWrapMode[stage]); }
FABLE_FORCEINLINE void CRenderStateManager::SetTextureBorderColour(long stage, unsigned long colour) { SetRenderState(colour, DeviceState.TextureBorderColour[stage]); }
FABLE_FORCEINLINE void CRenderStateManager::SetDepthOffset(float offset) { SetRenderState(*(unsigned long*)&offset, DeviceState.DepthOffset); }
FABLE_FORCEINLINE void CRenderStateManager::SetSlopeScaledDepthOffset(float offset) { SetRenderState(*(unsigned long*)&offset, DeviceState.DepthOffsetSlope); }

void CRenderStateManager::SetDefaultRenderStates()
{
    SetDitherEnable(true);
    SetZBufferEnable(true);
    SetZBufferWriteEnable(true);
    SetZBufferTestFunc(ZBUFFER_FUNC_LESS_EQUAL);
    SetPolyFillMode(POLY_FILL_SOLID);
    SetPolyShadeMode(POLY_SHADE_GOURAUD);
    SetAlphaTestEnable(false);
    SetAlphaRef(0x80);
    SetAlphaTestFunc(ALPHA_TEST_LESS);
    SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    SetDestAlphaBlendMode(ALPHA_BLEND_ZERO);
    SetPolyCullMode(POLY_CULL_CCW);
    SetAlphaBlendEnable(false);
    SetSpecularEnable(false);
    SetRenderState(0xFF000000, DeviceState.TextureFactor);   // FITTED (1): SetTextureFactor(0xFF000000) gives the same bytes but one more record
    SetTextureFactor(0xFFFFFFFF);
    SetClippingEnable(true);
    SetFogEnable(false);
    SetFogColour(0x00FFFFFF);
    SetFogMode(FOG_MODE_NONE);
    SetFogVertexMode(FOG_MODE_NONE);
    SetFogDensity(0.0f);
    SetFogStart(0.1f);
    SetFogEnd(0.9f);
    SetVertexBlending(VERTEX_BLEND_DISABLE);
    SetColourWriteEnable(true);
    SetDepthOffset(0.0f);
    SetSlopeScaledDepthOffset(0.0f);
    SetPointSpriteEnable(false);
    SetPointScaleEnable(false);
    SetPointSize(64.0f);
    SetPointSizeMin(1.0f);
    SetPointSizeMax(64.0f);
    SetPointScaleConstantFactor(1.0f);
    SetPointScaleDistanceFactor(0.0f);
    SetPointScaleDistanceSqrFactor(0.0f);
    SetBlendOp(BLEND_OP_ADD);
    SetLightingEnable(false);
    SetNormalizeNormals(false);
    for (long stage = 0; stage < 8; ++stage) {
        SetTextureOperation(stage, TEXTURE_OP_DISABLE);
        SetTextureAlphaOperation(stage, TEXTURE_OP_DISABLE);
        SetTextureArgument1(stage, TEXTURE_ARG_TEXTURE);
        SetTextureArgument2(stage, TEXTURE_ARG_DIFFUSE);
        SetTextureAlphaArgument1(stage, TEXTURE_ARG_TEXTURE);
        SetTextureAlphaArgument2(stage, TEXTURE_ARG_DIFFUSE);
        SetTextureFilterModeMin(stage, TEXTURE_FILTER_LINEAR);
        SetTextureFilterModeMag(stage, TEXTURE_FILTER_LINEAR);
        SetTextureUVModeU(stage, TEXTURE_UV_CLAMP);
        SetTextureUVModeV(stage, TEXTURE_UV_CLAMP);
        SetTextureUVModeW(stage, TEXTURE_UV_CLAMP);
        SetTextureMipmapLODBias(stage, 0.0f);
        SetTextureMipMapMode(stage, TEXTURE_MIPMAP_LINEAR);
        SetTextureCoordIndex(stage, stage);
        SetTextureTransformMode(stage, TEXTURE_TRANSFORM_DIABLED);
        SetTextureBumpEnvMatrix(stage, 0.0f, 0.0f, 0.0f, 0.0f);
        SetTextureCoordWrapUVW(stage, false, false, false);
        SetTextureMaxAnisotropy(stage, 1);
        SetTextureBorderColour(stage, 0);
    }
    SetTextureOperation(0, TEXTURE_OP_MODULATE);
    SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
    SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
    SetTextureArgument2(0, TEXTURE_ARG_DIFFUSE);
    SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
    SetTextureAlphaArgument2(0, TEXTURE_ARG_DIFFUSE);
    SetStencilEnable(false);
    SetStencilOpFail(STENCIL_OP_MODE_KEEP);
    SetStencilOpZFail(STENCIL_OP_MODE_KEEP);
    SetStencilOpPass(STENCIL_OP_MODE_KEEP);
    SetStencilFunc(STENCIL_FUNC_ALWAYS);
    SetStencilRef(0);
    SetStencilMask(0xFFFFFFFF);
    SetStencilWriteMask(0xFFFFFFFF);
    SetAmbientMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);
    SetDiffuseMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);
    SetSpecularMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);
}

