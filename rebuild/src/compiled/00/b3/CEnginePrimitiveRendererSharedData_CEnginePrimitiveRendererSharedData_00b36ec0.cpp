// CEnginePrimitiveRendererSharedData::~CEnginePrimitiveRendererSharedData() -- destroy the 40 embedded pixel shaders in reverse order (each releases its shared CPixelShaderData, deleting it at zero references), then the CBaseClassNonCopyable base
// Leaf: ??1CEnginePrimitiveRendererSharedData@@UAE@XZ
// Flags: none
// Grade: RELOCATION_MATCH (q.py with FABLE_HDR_OVERLAY=hdr/, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B36EC0, length 1341 bytes (+ int3 padding), __fastcall, tail jump to
//   ~CBaseClassNonCopyable 0x0099A300.
// Identity: Ego_r.pdb / FableWin.pdb ??1CEnginePrimitiveRendererSharedData@@UAE@XZ (virtual); its callers are the class's own
//   ??_G 0x00B37410 and ~CEnginePrimitiveRenderer 0x00B33520 (the shared data is the renderer's base/member). RTTI
//   CEnginePrimitiveRendererSharedData : CBaseClassNonCopyable : CBaseClass.
// Layout: Ego_r.pdb CEnginePrimitiveRendererSharedData (0x268 bytes, no containers, so retail = Ego_r): the forty CPixelShader members
//   run +0xDC .. +0x214 (8 bytes each: vptr + CPixelShaderData* Data); every other member is a scalar or a raw pointer.
// Member dtor: ~CPixelShader as in the landed ??_G 0x0098C3D0 (vtable 0x0129A054 store, Data null test, `dec [Data+8]`, deleting
//   virtual destructor of the data with 1, then the CShaderBase vtable 0x0129A030 store): retail keeps both vtables in edi/ebx.
// Shape: retail stores no CEnginePrimitiveRendererSharedData vtable first: the destructor is the implicit one (a user `~X() { }`
//   adds the 6-byte vtable store, 1351 v 1345). It is emitted out of line, as retail, by defining the PDB constructor
//   ??0CEnginePrimitiveRendererSharedData@@QAE@XZ in the TU (GOTCHAS chunk 710 lever); only the destructor is graded.
#include "engine/CBaseClassNonCopyable.h"

#include "engine/CPixelShader.h"                 // hand-back header (CPixelShaderData / CShaderBase / CPixelShader)

enum EEngineOcclusionLayer;                          // MS forward enums
enum EVertexBufferPool;
enum EIndexBufferPool;

class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
public:
    CEnginePrimitiveRendererSharedData();
    bool BlockOnLoad;                                           // +0x4
    unsigned char CurrentInterpolationInfo[0x70];               // +0x8 CEngineInterpolationInfo (no destructor)
    EEngineOcclusionLayer CurrentOcclusionLayer;                // +0x78
    float DrawDistanceFactor;                                   // +0x7c
    float DrawDistanceMinimum;                                  // +0x80
    float DrawDistanceMaximum;                                  // +0x84
    float RepeatedMeshDrawDistanceFactor;                       // +0x88
    float ZSpriteDrawDistanceFactor;                            // +0x8c
    float MinimumZSpriteAsMeshDistance;                         // +0x90
    float MaximumZSpriteAsMeshDistance;                         // +0x94
    bool DrawDistanceInvalid;                                   // +0x98
    float LODErrorTolerance;                                    // +0x9c
    float CharacterLODErrorTolerance;                           // +0xa0
    float LODVirtualScreenSize;                                 // +0xa4
    float LODVirtualScreenSizeFOVAdjusted;                      // +0xa8
    float MipmapQuality;                                        // +0xac
    float MipmapSizeMultiplier;                                 // +0xb0
    long PreloadMipmapTolerance;                                // +0xb4
    long PreloadMipmapMaxSize;                                  // +0xb8
    bool BumpMappingEnable;                                     // +0xbc
    bool OcclusionEnable;                                       // +0xbd
    bool OcclusionHierarchyEnable;                              // +0xbe
    bool OcclusionHierarchyEnableConsole;                       // +0xbf
    unsigned long GlobalPrimitiveInitFlags;                     // +0xc0
    unsigned long DynamicVBUsage;                               // +0xc4
    EVertexBufferPool DynamicVBPool;                            // +0xc8
    unsigned long DynamicVBLockFlags;                           // +0xcc
    unsigned long DynamicIBUsage;                               // +0xd0
    EIndexBufferPool DynamicIBPool;                             // +0xd4
    unsigned long DynamicIBLockFlags;                           // +0xd8
    CPixelShader PixelShaderDiffuseOnly;                        // +0xdc
    CPixelShader PixelShaderDiffuseColourTextureDiffuseAlpha;   // +0xe4
    CPixelShader PixelShaderTextureOnly;                        // +0xec
    CPixelShader PixelShaderDiffuse;                            // +0xf4
    CPixelShader PixelShaderDiffuseFog;                         // +0xfc
    CPixelShader PixelShaderDiffuseEnvFog;                      // +0x104
    CPixelShader PixelShaderDiffuseEnvBumpFog;                  // +0x10c
    CPixelShader PixelShaderDiffuseDP3BumpFog;                  // +0x114
    CPixelShader PixelShaderDiffuseFogDepthBufferShadow;        // +0x11c
    CPixelShader PixelShaderDiffuseFogDepthBufferShadowOutputFactor;// +0x124
    CPixelShader PixelShaderDiffuseEnvFogDepthBufferShadow;     // +0x12c
    CPixelShader PixelShaderDiffuseEnvBumpFogDepthBufferShadow; // +0x134
    CPixelShader PixelShaderDiffuseDP3BumpFogDepthBufferShadow; // +0x13c
    CPixelShader PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor;// +0x144
    CPixelShader PixelShaderDepthShadowBufferRender;            // +0x14c
    CPixelShader PixelShaderDepthShadowBufferRenderAlpha;       // +0x154
    CPixelShader PixelShaderColourShadowBufferRender;           // +0x15c
    CPixelShader PixelShaderColourShadowBufferRenderAlpha;      // +0x164
    CPixelShader PixelShaderDiffuseFogColourBufferShadow;       // +0x16c
    CPixelShader PixelShaderDiffuseFogColourBufferShadowOutputFactor;// +0x174
    CPixelShader PixelShaderDiffuseEnvFogColourBufferShadow;    // +0x17c
    CPixelShader PixelShaderDiffuseEnvBumpFogColourBufferShadow;// +0x184
    CPixelShader PixelShaderDiffuseDP3BumpFogColourBufferShadow;// +0x18c
    CPixelShader PixelShaderDiffuseDP3BumpFogColourBufferShadowOutputFactor;// +0x194
    CPixelShader PixelShader2DDiffuseOnly;                      // +0x19c
    CPixelShader PixelShader2DTextureOnly;                      // +0x1a4
    CPixelShader PixelShader2DDiffuse;                          // +0x1ac
    CPixelShader PixelShader2DDiffuseEnv;                       // +0x1b4
    CPixelShader PixelShader2DDiffuseEnvBump;                   // +0x1bc
    CPixelShader PixelShader2DDiffuseDP3Bump;                   // +0x1c4
    CPixelShader PixelShader2DDiffuseOnlyAdditiveAlpha;         // +0x1cc
    CPixelShader PixelShader2DTextureOnlyAdditiveAlpha;         // +0x1d4
    CPixelShader PixelShader2DDiffuseAdditiveAlpha;             // +0x1dc
    CPixelShader PixelShader2DDiffuseEnvAdditiveAlpha;          // +0x1e4
    CPixelShader PixelShader2DDiffuseEnvBumpAdditiveAlpha;      // +0x1ec
    CPixelShader PixelShader2DDiffuseDP3BumpAdditiveAlpha;      // +0x1f4
    CPixelShader PixelShaderOutlineSmoothFill;                  // +0x1fc
    CPixelShader PixelShaderOutlineSmoothFillAlpha;             // +0x204
    CPixelShader PixelShaderOutlineSmoothFillDP3;               // +0x20c
    CPixelShader PixelShaderOutlineSmoothFillDP3Alpha;          // +0x214
    CPixelShader* PixelShaderShadowBufferRender;                // +0x21c
    CPixelShader* PixelShaderShadowBufferRenderAlpha;           // +0x220
    CPixelShader* PixelShaderDiffuseFogShadow;                  // +0x224
    CPixelShader* PixelShaderDiffuseFogShadowOutputFactor;      // +0x228
    CPixelShader* PixelShaderDiffuseEnvFogShadow;               // +0x22c
    CPixelShader* PixelShaderDiffuseEnvBumpFogShadow;           // +0x230
    CPixelShader* PixelShaderDiffuseDP3BumpFogShadow;           // +0x234
    CPixelShader* PixelShaderDiffuseDP3BumpFogShadowOutputFactor;// +0x238
    float BoundingRadiusFix;                                    // +0x23c
    bool BoundingSphereViewEnabled;                             // +0x240
    bool BoundingBoxViewEnabled;                                // +0x241
    long ShowMeshInfo;                                          // +0x244
    bool ShowPreloadColours;                                    // +0x248
    long ShowVisibilityInfo;                                    // +0x24c
    long ShowSilhouettes;                                       // +0x250
    long ShowSilhouettesConsole;                                // +0x254
    bool VisibilityRatiosUpdate;                                // +0x258
    bool VisibilityRatiosReceiveUpdate;                         // +0x259
    bool VisibilityRatiosValid;                                 // +0x25a
    bool BoneViewEnabled;                                       // +0x25b
    bool Show2DBoundingRect;                                    // +0x25c
    bool Show2DBoundingSphere;                                  // +0x25d
    bool ShowUser2DBoundingSphere;                              // +0x25e
    bool Show2DRotationPos;                                     // +0x25f
    bool OldPixelShadersEnable;                                 // +0x260
    bool OldVertexShadersEnable;                                // +0x261
    bool HighlightMirroredTexturesEnabled;                      // +0x262
    long LODForced;                                             // +0x264
};

CEnginePrimitiveRendererSharedData::CEnginePrimitiveRendererSharedData()
{
}