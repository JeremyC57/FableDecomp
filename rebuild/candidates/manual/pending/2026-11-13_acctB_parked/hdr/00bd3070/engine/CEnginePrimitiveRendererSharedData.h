#ifndef FABLE_ENGINE_CENGINEPRIMITIVERENDERERSHAREDDATA_H
#define FABLE_ENGINE_CENGINEPRIMITIVERENDERERSHAREDDATA_H
// New header (chunk 731, 2026-11-03; one model, no class macro). RTTI: CEnginePrimitiveRenderer (vtable 0x012A1348) :
// CEnginePrimitiveRendererSharedData (+0) : CBaseClassNonCopyable : CBaseClass, with CEngineComponent at +0x268, so the shared
// data is 0x268 bytes in retail (CEngine::Initialise 0x00B26360 converts the renderer to CEngineComponent* as `p ? p + 0x268 : 0`).
// Own members are not modelled yet (one opaque block).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#if defined(FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS)
// Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): the render settings with their
// Ego_r.pdb names / types / offsets (no container before +0xC8, so retail = Ego_r; the row reads CurrentOcclusionLayer +0x78,
// RepeatedMeshDrawDistanceFactor +0x88, LODErrorTolerance +0x9C, LODVirtualScreenSizeFOVAdjusted +0xA8, OcclusionEnable +0xBD,
// BlockOnLoad +0x04) and the FableWin.pdb inline getters; the rest stays opaque. BlockOnLoad protected as in the STATICBLOCKS branch.
#include "engine/EEngineOcclusionLayer.h"
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SHADERS
#include "engine/CPixelShader.h"
#endif
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
public:
    EEngineOcclusionLayer GetCurrentOcclusionLayer() { return CurrentOcclusionLayer; }   // ?GetCurrentOcclusionLayer@CEnginePrimitiveRendererSharedData@@QAE?AW4EEngineOcclusionLayer@@XZ
    bool GetOcclusionEnable() { return OcclusionEnable; }                               // ?GetOcclusionEnable@CEnginePrimitiveRendererSharedData@@QAE_NXZ
    float GetRepeatedMeshDrawDistanceFactor() { return RepeatedMeshDrawDistanceFactor; } // ?GetRepeatedMeshDrawDistanceFactor@CEnginePrimitiveRendererSharedData@@QAEMXZ
protected:
    bool             BlockOnLoad;                                        // +0x04
    unsigned char    CurrentInterpolationInfo[0x70];                     // +0x08 CEngineInterpolationInfo
public:
    EEngineOcclusionLayer CurrentOcclusionLayer;                         // +0x78
    float            DrawDistanceFactor;                                 // +0x7c
    float            DrawDistanceMinimum;                                // +0x80
    float            DrawDistanceMaximum;                                // +0x84
    float            RepeatedMeshDrawDistanceFactor;                     // +0x88
    float            ZSpriteDrawDistanceFactor;                          // +0x8c
    float            MinimumZSpriteAsMeshDistance;                       // +0x90
    float            MaximumZSpriteAsMeshDistance;                       // +0x94
    bool             DrawDistanceInvalid;                                // +0x98
    float            LODErrorTolerance;                                  // +0x9c
    float            CharacterLODErrorTolerance;                         // +0xa0
    float            LODVirtualScreenSize;                               // +0xa4
    float            LODVirtualScreenSizeFOVAdjusted;                    // +0xa8
    float            MipmapQuality;                                      // +0xac
    float            MipmapSizeMultiplier;                               // +0xb0
    long             PreloadMipmapTolerance;                             // +0xb4
    long             PreloadMipmapMaxSize;                               // +0xb8
    bool             BumpMappingEnable;                                  // +0xbc
    bool             OcclusionEnable;                                    // +0xbd
    bool             OcclusionHierarchyEnable;                           // +0xbe
    bool             OcclusionHierarchyEnableConsole;                    // +0xbf
    unsigned long    GlobalPrimitiveInitFlags;                           // +0xc0
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
    // Nested opt-in (chunk 2032, row 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette): the debug / silhouette switches
    // with their Ego_r.pdb names and offsets (struct_layouts_egor.tsv; retail = Ego_r up to here: the row reads VisibilityRatiosValid
    // +0x25A at 0x00BA7026 and ShowSilhouettes +0x250 at 0x00BA7220); the shader block between stays opaque.
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SHADERS
    // Nested opt-in (chunk 4010 scout, row 0x00BD3070): the shader block with its Ego_r.pdb names (retail = Ego_r, no container: the row
    // reads MipmapSizeMultiplier +0xB0 (0x00BD3365), PixelShaderDiffuseOnly +0xDC, PixelShaderDiffuse +0xF4, DiffuseFog +0xFC, DiffuseEnvFog
    // +0x104, 2DDiffuse +0x1AC, the selector pointers +0x224..+0x238; landed DisplayMirroredTextures 0x00BA1B00 puts PixelShaderDiffuse at +0xF4).
    unsigned long    DynamicVBUsage;                                     // +0xc4
    long             DynamicVBPool;                                      // +0xc8 EVertexBufferPool
    unsigned long    DynamicVBLockFlags;                                 // +0xcc
    unsigned long    DynamicIBUsage;                                     // +0xd0
    long             DynamicIBPool;                                      // +0xd4 EIndexBufferPool
    unsigned long    DynamicIBLockFlags;                                 // +0xd8
    CPixelShader     PixelShaderDiffuseOnly, PixelShaderDiffuseColourTextureDiffuseAlpha, PixelShaderTextureOnly, PixelShaderDiffuse;   // +0xdc..
    CPixelShader     PixelShaderDiffuseFog, PixelShaderDiffuseEnvFog, PixelShaderDiffuseEnvBumpFog, PixelShaderDiffuseDP3BumpFog;          // +0xfc..
    CPixelShader     PixelShaderDiffuseFogDepthBufferShadow, PixelShaderDiffuseFogDepthBufferShadowOutputFactor;                            // +0x11c
    CPixelShader     PixelShaderDiffuseEnvFogDepthBufferShadow, PixelShaderDiffuseEnvBumpFogDepthBufferShadow;                               // +0x12c
    CPixelShader     PixelShaderDiffuseDP3BumpFogDepthBufferShadow, PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor;              // +0x13c
    CPixelShader     PixelShaderDepthShadowBufferRender, PixelShaderDepthShadowBufferRenderAlpha;                                           // +0x14c
    CPixelShader     PixelShaderColourShadowBufferRender, PixelShaderColourShadowBufferRenderAlpha;                                         // +0x15c
    CPixelShader     PixelShaderDiffuseFogColourBufferShadow, PixelShaderDiffuseFogColourBufferShadowOutputFactor;                          // +0x16c
    CPixelShader     PixelShaderDiffuseEnvFogColourBufferShadow, PixelShaderDiffuseEnvBumpFogColourBufferShadow;                            // +0x17c
    CPixelShader     PixelShaderDiffuseDP3BumpFogColourBufferShadow, PixelShaderDiffuseDP3BumpFogColourBufferShadowOutputFactor;            // +0x18c
    CPixelShader     PixelShader2DDiffuseOnly, PixelShader2DTextureOnly, PixelShader2DDiffuse, PixelShader2DDiffuseEnv;                    // +0x19c
    CPixelShader     PixelShader2DDiffuseEnvBump, PixelShader2DDiffuseDP3Bump;                                                              // +0x1bc
    CPixelShader     PixelShader2DDiffuseOnlyAdditiveAlpha, PixelShader2DTextureOnlyAdditiveAlpha, PixelShader2DDiffuseAdditiveAlpha;     // +0x1cc
    CPixelShader     PixelShader2DDiffuseEnvAdditiveAlpha, PixelShader2DDiffuseEnvBumpAdditiveAlpha, PixelShader2DDiffuseDP3BumpAdditiveAlpha;   // +0x1e4
    CPixelShader     PixelShaderOutlineSmoothFill, PixelShaderOutlineSmoothFillAlpha, PixelShaderOutlineSmoothFillDP3, PixelShaderOutlineSmoothFillDP3Alpha;   // +0x1fc
    CPixelShader*    PixelShaderShadowBufferRender;                      // +0x21c
    CPixelShader*    PixelShaderShadowBufferRenderAlpha;                 // +0x220
    CPixelShader*    PixelShaderDiffuseFogShadow;                        // +0x224
    CPixelShader*    PixelShaderDiffuseFogShadowOutputFactor;            // +0x228
    CPixelShader*    PixelShaderDiffuseEnvFogShadow;                     // +0x22c
    CPixelShader*    PixelShaderDiffuseEnvBumpFogShadow;                 // +0x230
    CPixelShader*    PixelShaderDiffuseDP3BumpFogShadow;                 // +0x234
    CPixelShader*    PixelShaderDiffuseDP3BumpFogShadowOutputFactor;     // +0x238
#else
    unsigned char    _pad_0xc4[0x23c - 0xc4];                            // +0xc4 DynamicVBUsage .. PixelShaderDiffuseDP3BumpFogShadowOutputFactor
#endif
    float            BoundingRadiusFix;                                  // +0x23c
    bool             BoundingSphereViewEnabled;                          // +0x240
    bool             BoundingBoxViewEnabled;                             // +0x241
    long             ShowMeshInfo;                                       // +0x244
    bool             ShowPreloadColours;                                 // +0x248
    long             ShowVisibilityInfo;                                 // +0x24c
    long             ShowSilhouettes;                                    // +0x250
    long             ShowSilhouettesConsole;                             // +0x254
    bool             VisibilityRatiosUpdate;                             // +0x258
    bool             VisibilityRatiosReceiveUpdate;                      // +0x259
    bool             VisibilityRatiosValid;                              // +0x25a
    bool             BoneViewEnabled;                                    // +0x25b
    bool             Show2DBoundingRect;                                 // +0x25c
    bool             Show2DBoundingSphere;                               // +0x25d
    bool             ShowUser2DBoundingSphere;                           // +0x25e
    bool             Show2DRotationPos;                                  // +0x25f
    bool             OldPixelShadersEnable;                              // +0x260
    bool             OldVertexShadersEnable;                             // +0x261
    bool             HighlightMirroredTexturesEnabled;                   // +0x262
    long             LODForced;                                          // +0x264
#else
    unsigned char    _pad_0xc4[0x268 - 0xc4];                            // +0xc4 DynamicVBUsage .. (not modelled here)
#endif
};
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS)
// Opt-in (chunk 851, row 0x00BA1B00 DisplayMirroredTextures): the members that row reads, Ego_r.pdb names at the Ego_r offsets (the
// record has no container before +0x268, so retail = Ego_r: DynamicIB* +0xD0..+0xD8 feed CIndexBuffer::Initialise / Lock,
// PixelShaderDiffuse +0xF4 goes to ApplyPixelShader, HighlightMirroredTexturesEnabled +0x262 gates the pass); the rest stays opaque.
#include "engine/CPixelShader.h"
enum EIndexBufferPool;                                                   // MS forward enum (engine/CIndexBuffer.h)
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
public:
    unsigned char    _pad_0x4[0xd0 - 0x4];                               // +0x04 members not modelled yet
    unsigned long    DynamicIBUsage;                                     // +0xd0
    EIndexBufferPool DynamicIBPool;                                      // +0xd4
    unsigned long    DynamicIBLockFlags;                                 // +0xd8
    CPixelShader     PixelShaderDiffuseOnly;                             // +0xdc
    CPixelShader     PixelShaderDiffuseColourTextureDiffuseAlpha;        // +0xe4
    CPixelShader     PixelShaderTextureOnly;                             // +0xec
    CPixelShader     PixelShaderDiffuse;                                 // +0xf4
    unsigned char    _pad_0xfc[0x262 - 0xfc];                            // +0xfc members not modelled yet
    bool             HighlightMirroredTexturesEnabled;                   // +0x262
    unsigned char    _pad_0x263[0x268 - 0x263];                          // +0x263 (LODForced +0x264)
};
#else
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
private:
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
protected:
    bool BlockOnLoad;
    unsigned char _pad_0x5[0x263];
#else
    unsigned char _pad_0x4[0x264];                                       // +0x04 members not modelled yet
#endif
};
#endif
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveRendererSharedData) == 0x268);
#endif // FABLE_ENGINE_CENGINEPRIMITIVERENDERERSHAREDDATA_H
