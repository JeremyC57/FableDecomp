#ifndef FABLE_ENGINE_CENGINESHADOWRENDERER_H
#define FABLE_ENGINE_CENGINESHADOWRENDERER_H
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineShadowRenderer@@ (vtable 0x012A2374) : CEngineComponent (+0x0); sizeof 0x340 from
// CEngine::Initialise 0x00B26360 (`new` of 0x340 bytes before the constructor call). Constructor ??0CEngineShadowRenderer@@QAE@XZ 0x00B50370 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineShadowRenderer@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CEngineComponent.h"
class CEngineInit;
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
// Retail opt-in (chunk 2024, row 0x00B4FC40 SetupShadowFuncRenderStates(long, const CMatrix3x4&, const CColorBufferDepthPackParameters&)):
// the own members in Ego_r.pdb order, names and types (struct_layouts_egor.tsv; no container member and C3DVector is 1-aligned, so
// the Ego_r offsets are retail's: the row reads SceneData.WorldToShadowBufferMatrixTextureSpace +0x8C..+0xC8, ShadowFadeStart /
// ShadowFadeEnd +0xFC / +0x100, ShadowBufferTexture +0x2E8, DepthPackingLookupTexture +0x32C and RenderingTechnique +0x334). The nested
// CColorBufferDepthPackParameters (Ego_r.pdb, 12 bytes, U-keyed: ?SetupShadowFuncRenderStates@CEngineShadowRenderer@@QAEXJABUCColorBufferDepthPackParameters@1@@Z)
// and EShadowRenderingTechnique (nested: ?PeekRenderingTechnique@CEngineShadowRenderer@@QBE?AW4EShadowRenderingTechnique@1@XZ; the
// enumerator names are descriptive, the PDB keeps only the tag; 0 / 1 / 2 select the ice pass's plain / shadow-buffer / colour-buffer
// shaders at 0x00B75B04, hand-back 2026-11-03_linux_hand727). Inline accessors: FableWin.pdb only (no Ego_r.pdb or Fable.exe copy).
#ifndef FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE
#endif
#ifndef FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE
#endif
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CMatrix4x4.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderTarget.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
class CEngineShadowRenderer : public CEngineComponent {
public:
    struct CColorBufferDepthPackParameters {                             // 12 bytes (Ego_r.pdb)
        CColorBufferDepthPackParameters(long stage, long carry_constant, long repeat_constant)   // FableWin ??0CColorBufferDepthPackParameters@CEngineShadowRenderer@@QAE@JJJ@Z
            : DepthToColourLookupTextureStage(stage), LowByteCarryPixelShaderConstant(carry_constant),
              LowRepeatFactorUserVertexShaderConstant(repeat_constant) { }
        long DepthToColourLookupTextureStage;                            // +0x00
        long LowByteCarryPixelShaderConstant;                            // +0x04
        long LowRepeatFactorUserVertexShaderConstant;                    // +0x08
    };
    enum EShadowRenderingTechnique {
        SHADOW_RENDERING_TECHNIQUE_NONE,                                 // descriptive (0: unshadowed shaders)
        SHADOW_RENDERING_TECHNIQUE_SHADOW_BUFFER,                        // descriptive (1: the "Shadowed" shaders)
        SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER                         // descriptive (2: the "ColbuffShadowed" shaders)
    };
    CEngineShadowRenderer();                   // 0x00B50370 ??0CEngineShadowRenderer@@QAE@XZ
    virtual unsigned long PeekSceneFilterFlags() const;
    void SetupShadowFuncRenderStates(long stage, const CMatrix3x4& texture_transform,
                                     const CColorBufferDepthPackParameters& pack_parameters);   // 0x00B4FC40
    void SetupShadowFuncRenderStates(long stage, const CColorBufferDepthPackParameters& pack_parameters);   // 0x00B50510
    EShadowRenderingTechnique PeekRenderingTechnique() const { return RenderingTechnique; }
    CTexture& GetShadowBufferTexture() { return ShadowBufferTexture; }
    CTexture& GetDepthPackingTexture() { return DepthPackingLookupTexture; }
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
    // Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): Ego_r.pdb / FableWin.pdb
    // ?PeekSceneData@CEngineShadowRenderer@@QAE?BVCShadowRendererSceneData@@XZ (inline; the caller copies SceneData +0x0C through the
    // out-of-line copy constructor 0x00B38600 into its own frame, 0x00B9A71D..0x00B9A72B).
    const CShadowRendererSceneData PeekSceneData() { return SceneData; }
#endif
private:
    CShadowRendererSceneData  SceneData;                                 // +0x00c
    CRenderStateCaptureBlock  ShadowBufferRenderStateCaptureBlock;       // +0x24c
    float                     ShadowProjectionZOffset;                   // +0x250
    float                     ShadowProjectionZSlopeOffset;              // +0x254
    float                     ColourBufferShadowTextureZBias;            // +0x258
    float                     ColourBufferShadowMinWClamp;               // +0x25c
    bool                      EnableShadowBufferDraw;                    // +0x260
    C3DVector                 StaticDataOrigin;                          // +0x261
    C3DVector                 CameraLookVector;                          // +0x26d
    CMatrix3x4                CameraViewMatrix;                          // +0x27c
    bool                      PerspectiveShadowsEnabled;                 // +0x2ac
    bool                      SceneOriginLocked;                         // +0x2ad
    bool                      EnableFrustrumDebugDisplay;                // +0x2ae
    bool                      TexturesAllocated;                         // +0x2af
    float                     PerspectiveInterpStart;                    // +0x2b0
    float                     PerspectiveInterpEnd;                      // +0x2b4
    float                     ForegroundBufferSpan;                      // +0x2b8
    float                     MaxShadowDistance;                         // +0x2bc
    float                     ShadowFadeRatio;                           // +0x2c0
    CRenderTarget             RenderTarget;                              // +0x2c4
    CTexture                  ShadowBufferTexture;                       // +0x2e8
    CTexture                  RenderSurface;                             // +0x2f0
    CSurface                  ShadowBufferDepthSurface;                  // +0x2f8
    CSurface                  ShadowBufferColourSurface;                 // +0x308
    CTexture                  ShadowBufferColourTexture;                 // +0x318
    C3DVector                 ShadowBufferTextureUVScaler;               // +0x320
    CTexture                  DepthPackingLookupTexture;                 // +0x32c
    EShadowRenderingTechnique RenderingTechnique;                        // +0x334
    long                      UserVideoOptionShadowBufferSize;           // +0x338
    float                     UserVideoOptionShadowDistanceScale;        // +0x33c
};
#else
class CEngineShadowRenderer : public CEngineComponent {
public:
    CEngineShadowRenderer();                   // 0x00B50370 ??0CEngineShadowRenderer@@QAE@XZ
    virtual unsigned long PeekSceneFilterFlags() const;
private:
    unsigned char _pad_0xc[0x334];              // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
};
#endif
FABLE_STATIC_ASSERT(sizeof(CEngineShadowRenderer) == 0x340);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineShadowRenderer = 0x340.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct IDirect3DTexture9;

#pragma pack(push, 1)
class CEngineShadowRenderer {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _base_0x04[0x8];                           // +0x04 flattened base-class subobject(s)
    unsigned char SceneData[0x240];                          // +0x0c CShadowRendererSceneData
    unsigned char ShadowBufferRenderStateCaptureBlock[0x4];  // +0x24c CRenderStateCaptureBlock
    float         ShadowProjectionZOffset;                   // +0x250
    float         ShadowProjectionZSlopeOffset;              // +0x254
    float         ColourBufferShadowTextureZBias;            // +0x258
    float         ColourBufferShadowMinWClamp;               // +0x25c
    bool          EnableShadowBufferDraw;                    // +0x260
    float         StaticDataOrigin_X;                        // +0x261 C3DVector::X
    float         StaticDataOrigin_Y;                        // +0x265 C3DVector::Y
    float         StaticDataOrigin_Z;                        // +0x269 C3DVector::Z
    unsigned char CameraLookVector[0xf];                     // +0x26d C3DVector
    unsigned char CameraViewMatrix[0x30];                    // +0x27c CMatrix3x4
    bool          PerspectiveShadowsEnabled;                 // +0x2ac
    bool          SceneOriginLocked;                         // +0x2ad
    bool          EnableFrustrumDebugDisplay;                // +0x2ae
    bool          TexturesAllocated;                         // +0x2af
    float         PerspectiveInterpStart;                    // +0x2b0
    float         PerspectiveInterpEnd;                      // +0x2b4
    float         ForegroundBufferSpan;                      // +0x2b8
    float         MaxShadowDistance;                         // +0x2bc
    float         ShadowFadeRatio;                           // +0x2c0
    unsigned char RenderTarget[0x24];                        // +0x2c4 CRenderTarget
    unsigned char ShadowBufferTexture[0x8];                  // +0x2e8 CTexture
    unsigned char RenderSurface[0x8];                        // +0x2f0 CTexture
    unsigned char ShadowBufferDepthSurface[0x10];            // +0x2f8 CSurface
    unsigned char ShadowBufferColourSurface[0x10];           // +0x308 CSurface
    unsigned char ShadowBufferColourTexture[0x8];            // +0x318 CTexture
    float         ShadowBufferTextureUVScaler_X;             // +0x320 C3DVector::X
    float         ShadowBufferTextureUVScaler_Y;             // +0x324 C3DVector::Y
    float         ShadowBufferTextureUVScaler_Z;             // +0x328 C3DVector::Z
    unsigned char DepthPackingLookupTexture[0x8];            // +0x32c CTexture
    unsigned char RenderingTechnique[0x4];                   // +0x334 EShadowRenderingTechnique
    long          UserVideoOptionShadowBufferSize;           // +0x338
    float         UserVideoOptionShadowDistanceScale;        // +0x33c
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineShadowRenderer) == 0x340);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, SceneData) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowBufferRenderStateCaptureBlock) == 0x24c);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowProjectionZOffset) == 0x250);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowProjectionZSlopeOffset) == 0x254);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ColourBufferShadowTextureZBias) == 0x258);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ColourBufferShadowMinWClamp) == 0x25c);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, EnableShadowBufferDraw) == 0x260);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, StaticDataOrigin_X) == 0x261);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, StaticDataOrigin_Y) == 0x265);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, StaticDataOrigin_Z) == 0x269);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, CameraLookVector) == 0x26d);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, CameraViewMatrix) == 0x27c);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, PerspectiveShadowsEnabled) == 0x2ac);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, SceneOriginLocked) == 0x2ad);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, EnableFrustrumDebugDisplay) == 0x2ae);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, TexturesAllocated) == 0x2af);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, PerspectiveInterpStart) == 0x2b0);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, PerspectiveInterpEnd) == 0x2b4);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ForegroundBufferSpan) == 0x2b8);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, MaxShadowDistance) == 0x2bc);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowFadeRatio) == 0x2c0);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, RenderTarget) == 0x2c4);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowBufferTexture) == 0x2e8);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, RenderSurface) == 0x2f0);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowBufferDepthSurface) == 0x2f8);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowBufferColourSurface) == 0x308);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowBufferColourTexture) == 0x318);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowBufferTextureUVScaler_X) == 0x320);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowBufferTextureUVScaler_Y) == 0x324);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, ShadowBufferTextureUVScaler_Z) == 0x328);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, DepthPackingLookupTexture) == 0x32c);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, RenderingTechnique) == 0x334);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, UserVideoOptionShadowBufferSize) == 0x338);
FABLE_STATIC_ASSERT(offsetof(CEngineShadowRenderer, UserVideoOptionShadowDistanceScale) == 0x33c);

#endif // FABLE_RETAIL_CENGINESHADOWRENDERER
#endif // FABLE_ENGINE_CENGINESHADOWRENDERER_H
