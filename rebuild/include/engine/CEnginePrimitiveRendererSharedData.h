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
enum EVertexBufferPool;                                                  // MS forward enums
enum EIndexBufferPool;
#endif
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_DTOR
    // Opt-in (NF46 round 7, row 0x00B33520 ~CEnginePrimitiveRenderer): Ego_r.pdb / FableWin.pdb ??1CEnginePrimitiveRendererSharedData@@UAE@XZ
    // (public virtual; retail 0x00B36EC0, called out of line by the derived destructor). Declaration only: no layout change.
    virtual ~CEnginePrimitiveRendererSharedData();                       // 0x00B36EC0
#endif

#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Original Ego_r/FableWin TPI and module916 call sites; private evidence in00ba115c.
 float GetDrawDistanceFactor(){return DrawDistanceFactor;}
#endif
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
    // Nested opt-in (wfE7, row 0x00B33B50 CEnginePrimitiveRenderer::CEnginePrimitiveRenderer; needs _SILHOUETTES): the dynamic buffer
    // settings and the forty CPixelShader members + eight CPixelShader* with their Ego_r.pdb names / types / offsets (retail = Ego_r: the
    // ctor stores DynamicVBPool +0xC8 ... and assigns every shader at its Ego_r offset, 0x00B34641..0x00B35C5C), and the out-of-line
    // ??0CEnginePrimitiveRendererSharedData@@QAE@XZ (retail 0x00B36CE0, landed; Ego_r / FableWin).
public:
    CEnginePrimitiveRendererSharedData();                                // 0x00B36CE0
    unsigned long    DynamicVBUsage;                                      // +0xc4
    EVertexBufferPool DynamicVBPool;                                       // +0xc8
    unsigned long    DynamicVBLockFlags;                                  // +0xcc
    unsigned long    DynamicIBUsage;                                      // +0xd0
    EIndexBufferPool DynamicIBPool;                                       // +0xd4
    unsigned long    DynamicIBLockFlags;                                  // +0xd8
    CPixelShader     PixelShaderDiffuseOnly;                              // +0xdc
    CPixelShader     PixelShaderDiffuseColourTextureDiffuseAlpha;         // +0xe4
    CPixelShader     PixelShaderTextureOnly;                              // +0xec
    CPixelShader     PixelShaderDiffuse;                                  // +0xf4
    CPixelShader     PixelShaderDiffuseFog;                               // +0xfc
    CPixelShader     PixelShaderDiffuseEnvFog;                            // +0x104
    CPixelShader     PixelShaderDiffuseEnvBumpFog;                        // +0x10c
    CPixelShader     PixelShaderDiffuseDP3BumpFog;                        // +0x114
    CPixelShader     PixelShaderDiffuseFogDepthBufferShadow;              // +0x11c
    CPixelShader     PixelShaderDiffuseFogDepthBufferShadowOutputFactor;  // +0x124
    CPixelShader     PixelShaderDiffuseEnvFogDepthBufferShadow;           // +0x12c
    CPixelShader     PixelShaderDiffuseEnvBumpFogDepthBufferShadow;       // +0x134
    CPixelShader     PixelShaderDiffuseDP3BumpFogDepthBufferShadow;       // +0x13c
    CPixelShader     PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor; // +0x144
    CPixelShader     PixelShaderDepthShadowBufferRender;                  // +0x14c
    CPixelShader     PixelShaderDepthShadowBufferRenderAlpha;             // +0x154
    CPixelShader     PixelShaderColourShadowBufferRender;                 // +0x15c
    CPixelShader     PixelShaderColourShadowBufferRenderAlpha;            // +0x164
    CPixelShader     PixelShaderDiffuseFogColourBufferShadow;             // +0x16c
    CPixelShader     PixelShaderDiffuseFogColourBufferShadowOutputFactor; // +0x174
    CPixelShader     PixelShaderDiffuseEnvFogColourBufferShadow;          // +0x17c
    CPixelShader     PixelShaderDiffuseEnvBumpFogColourBufferShadow;      // +0x184
    CPixelShader     PixelShaderDiffuseDP3BumpFogColourBufferShadow;      // +0x18c
    CPixelShader     PixelShaderDiffuseDP3BumpFogColourBufferShadowOutputFactor; // +0x194
    CPixelShader     PixelShader2DDiffuseOnly;                            // +0x19c
    CPixelShader     PixelShader2DTextureOnly;                            // +0x1a4
    CPixelShader     PixelShader2DDiffuse;                                // +0x1ac
    CPixelShader     PixelShader2DDiffuseEnv;                             // +0x1b4
    CPixelShader     PixelShader2DDiffuseEnvBump;                         // +0x1bc
    CPixelShader     PixelShader2DDiffuseDP3Bump;                         // +0x1c4
    CPixelShader     PixelShader2DDiffuseOnlyAdditiveAlpha;               // +0x1cc
    CPixelShader     PixelShader2DTextureOnlyAdditiveAlpha;               // +0x1d4
    CPixelShader     PixelShader2DDiffuseAdditiveAlpha;                   // +0x1dc
    CPixelShader     PixelShader2DDiffuseEnvAdditiveAlpha;                // +0x1e4
    CPixelShader     PixelShader2DDiffuseEnvBumpAdditiveAlpha;            // +0x1ec
    CPixelShader     PixelShader2DDiffuseDP3BumpAdditiveAlpha;            // +0x1f4
    CPixelShader     PixelShaderOutlineSmoothFill;                        // +0x1fc
    CPixelShader     PixelShaderOutlineSmoothFillAlpha;                   // +0x204
    CPixelShader     PixelShaderOutlineSmoothFillDP3;                     // +0x20c
    CPixelShader     PixelShaderOutlineSmoothFillDP3Alpha;                // +0x214
    CPixelShader*    PixelShaderShadowBufferRender;                       // +0x21c
    CPixelShader*    PixelShaderShadowBufferRenderAlpha;                  // +0x220
    CPixelShader*    PixelShaderDiffuseFogShadow;                         // +0x224
    CPixelShader*    PixelShaderDiffuseFogShadowOutputFactor;             // +0x228
    CPixelShader*    PixelShaderDiffuseEnvFogShadow;                      // +0x22c
    CPixelShader*    PixelShaderDiffuseEnvBumpFogShadow;                  // +0x230
    CPixelShader*    PixelShaderDiffuseDP3BumpFogShadow;                  // +0x234
    CPixelShader*    PixelShaderDiffuseDP3BumpFogShadowOutputFactor;      // +0x238
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SHADOWBUFFERSHADERS)
    // Nested opt-in (wfA6b, row 0x00BA6240 CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer): Ego_r.pdb
    // PixelShaderShadowBufferRender +0x21C / PixelShaderShadowBufferRenderAlpha +0x220 (CPixelShader*) carved out of the pad.
    // Same size and later offsets.
    unsigned char    _pad_0xc4[0x21c - 0xc4];                            // +0xc4 DynamicVBUsage .. PixelShaderOutlineSmoothFillDP3Alpha
    class CPixelShader* PixelShaderShadowBufferRender;                   // +0x21c
    class CPixelShader* PixelShaderShadowBufferRenderAlpha;              // +0x220
    unsigned char    _pad_0x224[0x23c - 0x224];                          // +0x224 PixelShaderDiffuseFogShadow .. PixelShaderDiffuseDP3BumpFogShadowOutputFactor
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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#include "engine/EEngineOcclusionLayer.h"
#endif
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
    // Nested opt-in (bwf1 / agentC2, row 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive): Ego_r.pdb CurrentOcclusionLayer +0x78,
    // MipmapSizeMultiplier +0xB0 and OcclusionEnable +0xBD (retail 0x00BB3EC9 / 0x00BB3EEF; no container before them: retail = Ego_r),
    // with the FableWin.pdb inline getters, plus the outline / shadow pixel shaders below (+0x1FC .. +0x228, Ego_r offsets).
    bool             BlockOnLoad;                                        // +0x04
    unsigned char    _pad_0x5[0x78 - 0x5];                               // +0x05 members not modelled yet
    EEngineOcclusionLayer CurrentOcclusionLayer;                         // +0x78
    unsigned char    _pad_0x7c[0xb0 - 0x7c];                             // +0x7c members not modelled yet
    float            MipmapSizeMultiplier;                               // +0xb0
    unsigned char    _pad_0xb4[0xbd - 0xb4];                             // +0xb4 members not modelled yet
    bool             OcclusionEnable;                                    // +0xbd
    unsigned char    _pad_0xbe[0xd0 - 0xbe];                             // +0xbe members not modelled yet
    EEngineOcclusionLayer GetCurrentOcclusionLayer() { return CurrentOcclusionLayer; }   // ?GetCurrentOcclusionLayer@CEnginePrimitiveRendererSharedData@@QAE?AW4EEngineOcclusionLayer@@XZ
    bool GetOcclusionEnable() { return OcclusionEnable; }                // ?GetOcclusionEnable@CEnginePrimitiveRendererSharedData@@QAE_NXZ
#else
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_DYNAMICVB
    // Nested opt-in (agentC22, row 0x00BCC800 RenderWeaponTrail): Ego_r.pdb DynamicVBUsage +0xC4 / DynamicVBPool +0xC8 / DynamicVBLockFlags
    // +0xCC (struct_layouts_egor.tsv), read by the inline CEnginePrimitiveRenderUtil::GetDynamicVB* getters (FableWin.pdb).
    unsigned char    _pad_0x4[0xc4 - 0x4];                               // +0x04 members not modelled yet
    unsigned long    DynamicVBUsage;                                     // +0xc4
    enum EVertexBufferPool DynamicVBPool;                                // +0xc8
    unsigned long    DynamicVBLockFlags;                                 // +0xcc
#else
    unsigned char    _pad_0x4[0xd0 - 0x4];                               // +0x04 members not modelled yet
#endif
#endif
    unsigned long    DynamicIBUsage;                                     // +0xd0
    EIndexBufferPool DynamicIBPool;                                      // +0xd4
    unsigned long    DynamicIBLockFlags;                                 // +0xd8
    CPixelShader     PixelShaderDiffuseOnly;                             // +0xdc
    CPixelShader     PixelShaderDiffuseColourTextureDiffuseAlpha;        // +0xe4
    CPixelShader     PixelShaderTextureOnly;                             // +0xec
    CPixelShader     PixelShaderDiffuse;                                 // +0xf4
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH2D
    // Nested opt-in (agentC6, row 0x00BB30A0 Render2DClothPrimitive): the 2D pixel shaders, Ego_r.pdb names / offsets (no container before
    // them: retail = Ego_r; the row applies +0x1AC at 0x00BB3A30).
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SETUPRENDERMODE
    // Nested opt-in (agentC16, row 0x00BA2350 SetupRenderModeShadersAndConstants; needs _CLOTH and _CLOTH2D): the pixel shaders that row
    // applies, Ego_r.pdb names / offsets (struct_layouts_egor; no container before them: retail = Ego_r).
    CPixelShader     PixelShaderDiffuseFog;                              // +0xfc
    CPixelShader     PixelShaderDiffuseEnvFog;                           // +0x104
    unsigned char    _pad_0x10c[0x19c - 0x10c];                          // +0x10c members not modelled yet
#else
    unsigned char    _pad_0xfc[0x19c - 0xfc];                            // +0xfc members not modelled yet
#endif
    CPixelShader     PixelShader2DDiffuseOnly;                           // +0x19c
    CPixelShader     PixelShader2DTextureOnly;                           // +0x1a4
    CPixelShader     PixelShader2DDiffuse;                               // +0x1ac
    CPixelShader     PixelShader2DDiffuseEnv;                            // +0x1b4
    CPixelShader     PixelShader2DDiffuseEnvBump;                        // +0x1bc
    CPixelShader     PixelShader2DDiffuseDP3Bump;                        // +0x1c4
    CPixelShader     PixelShader2DDiffuseOnlyAdditiveAlpha;              // +0x1cc
    CPixelShader     PixelShader2DTextureOnlyAdditiveAlpha;              // +0x1d4
    CPixelShader     PixelShader2DDiffuseAdditiveAlpha;                  // +0x1dc
    CPixelShader     PixelShader2DDiffuseEnvAdditiveAlpha;               // +0x1e4
    CPixelShader     PixelShader2DDiffuseEnvBumpAdditiveAlpha;           // +0x1ec
    CPixelShader     PixelShader2DDiffuseDP3BumpAdditiveAlpha;           // +0x1f4
#else
    unsigned char    _pad_0xfc[0x1fc - 0xfc];                            // +0xfc members not modelled yet
#endif
    CPixelShader     PixelShaderOutlineSmoothFill;                       // +0x1fc
    CPixelShader     PixelShaderOutlineSmoothFillAlpha;                  // +0x204
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SETUPRENDERMODE
    CPixelShader     PixelShaderOutlineSmoothFillDP3;                    // +0x20c
    CPixelShader     PixelShaderOutlineSmoothFillDP3Alpha;               // +0x214
    unsigned char    _pad_0x21c[0x224 - 0x21c];                          // +0x21c PixelShaderShadowBufferRender[Alpha]*
#else
    unsigned char    _pad_0x20c[0x224 - 0x20c];                          // +0x20c members not modelled yet
#endif
    CPixelShader*    PixelShaderDiffuseFogShadow;                        // +0x224
    CPixelShader*    PixelShaderDiffuseFogShadowOutputFactor;            // +0x228
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SETUPRENDERMODE
    CPixelShader*    PixelShaderDiffuseEnvFogShadow;                     // +0x22c
    CPixelShader*    PixelShaderDiffuseEnvBumpFogShadow;                 // +0x230
    CPixelShader*    PixelShaderDiffuseDP3BumpFogShadow;                 // +0x234
    CPixelShader*    PixelShaderDiffuseDP3BumpFogShadowOutputFactor;     // +0x238
    unsigned char    _pad_0x23c[0x262 - 0x23c];                          // +0x23c members not modelled yet
#else
    unsigned char    _pad_0x22c[0x262 - 0x22c];                          // +0x22c members not modelled yet
#endif
#else
    unsigned char    _pad_0xfc[0x262 - 0xfc];                            // +0xfc members not modelled yet
#endif
    bool             HighlightMirroredTexturesEnabled;                   // +0x262
    unsigned char    _pad_0x263[0x268 - 0x263];                          // +0x263 (LODForced +0x264)
};
#else
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
private:
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
protected:
    bool BlockOnLoad;
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_ZSPRITE
    // Overlay opt-in (row 0x00BAA870 RenderFromSortList): Ego_r.pdb ZSpriteDrawDistanceFactor +0x8C (read in place 0x00BAB394),
    // MinimumZSpriteAsMeshDistance +0x90 / MaximumZSpriteAsMeshDistance +0x94 and MipmapSizeMultiplier +0xb0 (superset of
    // _MIPMAPSIZEMULTIPLIER). FableWin.pdb inlines ?GetZSpriteDrawDistanceFactor / ?GetMinimumZSpriteAsMeshDistance /
    // ?GetMaximumZSpriteAsMeshDistance@CEnginePrimitiveRendererSharedData@@QAEMXZ. No layout change.
    unsigned char _pad_0x5[0x8c - 0x5];
public:
    float ZSpriteDrawDistanceFactor;                                     // +0x8c
    float MinimumZSpriteAsMeshDistance;                                  // +0x90
    float MaximumZSpriteAsMeshDistance;                                  // +0x94
protected:
#ifdef FABLE_RETAIL_LOCAL_DETAIL_RENDER_PASS29
    bool DrawDistanceInvalid;
    unsigned char _pad_0x99[0xb0 - 0x99];
public:
    float IsDrawDistanceInvalid() { return DrawDistanceInvalid; }
#else
    unsigned char _pad_0x98[0xb0 - 0x98];
#endif
public:
    float MipmapSizeMultiplier;                                          // +0xb0
    float GetZSpriteDrawDistanceFactor() { return ZSpriteDrawDistanceFactor; }
    float GetMinimumZSpriteAsMeshDistance() { return MinimumZSpriteAsMeshDistance; }
    float GetMaximumZSpriteAsMeshDistance() { return MaximumZSpriteAsMeshDistance; }
protected:
    unsigned char _pad_0xb4[0x268 - 0xb4];
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MIPMAPSIZEMULTIPLIER)
    // Opt-in (row 0x00B8B780): Ego_r.pdb MipmapSizeMultiplier +0xb0 (float), read by the inline
    // CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier.
    unsigned char _pad_0x5[0xb0 - 0x5];
public:
    float MipmapSizeMultiplier;                                          // +0xb0
protected:
    unsigned char _pad_0xb4[0x268 - 0xb4];
#else
    unsigned char _pad_0x5[0x263];
#endif
#else
    unsigned char _pad_0x4[0x264];                                       // +0x04 members not modelled yet
#endif
};
#endif
FABLE_STATIC_ASSERT_AT(107, sizeof(CEnginePrimitiveRendererSharedData) == 0x268);
#endif // FABLE_ENGINE_CENGINEPRIMITIVERENDERERSHAREDDATA_H
